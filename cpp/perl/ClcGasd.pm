use strict;
use warnings;
package ClcGasd;
###########################################################################
sub new     #01.09.2005 13:04
############################################################################
 {
    my ($ClcName, $ShowE, $ShowT, $ClcLayer, $ClcTime, $TimeCoef) = @_;
    my $Params = PackParams($ClcName, $ShowE, $ShowT, $ClcLayer, $ClcTime, $TimeCoef);
#    my $Head = geobase_head::new(); my ($r) = { "Head" =>$Head };
    bless $Params, "ClcGasd";
    return $Params;
}   ##new

############################################################################
sub ClcAssembly		#09/19/2007 11:07
############################################################################
 {
	my ($Par) = @_;

	my @ClcLayer = @{$Par->{ClcLayer}};
	my $ClcTime = $Par->{ClcTime};
	my @Bnd;
	$Bnd[0] = 0;
	my $Matter_Boundaries="";
	my $Matters="";
	my $Parameters="";
	for(my $it=0;$it<int(@ClcLayer);$it++){
PrintHash($ClcLayer[$it]);
		$Bnd[$it+1] = $Bnd[$it] + $ClcLayer[$it]{N};
		$Matter_Boundaries = $Matter_Boundaries."  ".$Bnd[$it+1]	if ($it!=int(@ClcLayer)-1);
		$Matters = $Matters."\n"				if ($it>0);
		$Matters = $Matters.$ClcLayer[$it]{M};
		my $t = $it+1;
		$Parameters = $Parameters." P[$t]   $ClcLayer[$it]{P}  D[$t] $ClcLayer[$it]{D}     E[$t]  $ClcLayer[$it]{E}    V[$t]  $ClcLayer[$it]{V} Length[$t] $ClcLayer[$it]{L}\n";
	}
	my ($TimeStp, $TimeWrite, $EndTime) = ( $ClcTime->{TimeStp}*1e-8, $ClcTime->{TimeWrite}*1e-8, $ClcTime->{EndTime}*1e-8 );
        open(OUT,">$Par->{ClcName}")            or die "Could not open file $Par->{ClcName}:$!\n";
	print OUT <<EOF
BegTime 0 TimeStp $TimeStp TimeWrite $TimeWrite PresDerivCoef 0.7 EndTime $EndTime
NumPnt   $Bnd[$#Bnd]  LftPnt   0  RgtPnt   1  NumIntPar $ClcTime->{NumIntPar}
LftBnd_Free 1 RgtBnd_Free 1 TimeStability $ClcTime->{TimeStability}
Viscouse 0.7  SecondVisc 0
NumMatters $#Bnd Matter_Boundaries $Matter_Boundaries
$Matters
bad
Parameters
$Parameters
EOF
	;close(OUT);
	system("cell_dat.exe $Par->{ClcName} $Par->{ClcName}.ck /l");
	system("cell_kru.exe $Par->{ClcName}.ck $Par->{ClcName}.dat");

}	##ClcAssembly

############################################################################
sub GetPntE		#09/19/2007 11:11
############################################################################
 {
    my ($Par, $PntNum) = @_;
	my ($tmp_name, $conf_name, $res_name) = ("tmp.dat", "show.cfg", "show_pnt.dat");
	my $IntStr = "";
	for(my $i = 0;$i<$Par->{ClcTime}{NumIntPar};$i++){
		$IntStr = "$IntStr Name1 Velocity  Color1 13 Show 1\n";
	}
	my $NumCurve = 5 + $Par->{ClcTime}{NumIntPar};
	open(OUT,">$conf_name");
	print OUT <<EOF
NumCurve $NumCurve XCurve  1
x_min  0  x_max  11 y_min  0  y_max  10
Name1 Pos  Color1 10 Show 1
Name1 Pressure  Color1 10 Show 1
Name1 Dencity  Color1 11 Show 1
Name1 Energy  Color1 12 Show 1
Name1 Velocity  Color1 13 Show 1
$IntStr
EOF
	;unlink($tmp_name);
	rename("$Par->{ClcName}.dat", $tmp_name);
	system("show.exe $tmp_name $conf_name /g$PntNum");
    rename($tmp_name, "$Par->{ClcName}.dat");
    if (!defined($Par->{TimeCoef})){
        rename($res_name, "$Par->{ClcName}.$PntNum");
    } else {
        open II, "<$res_name"           or die "Could not open in file $res_name:$!\n";
        open OO, ">$Par->{ClcName}.$PntNum"           or die "Could not open out file $Par->{ClcName}.$PntNum:$!\n";
        my $n = "\x0D\x0A";
        while(<II>){
            my @l = split;
            print OO ($_), next     if (!IsNumber($l[0]));
            $l[0]*=$Par->{TimeCoef};
            print OO join("\t ",@l),"$n";
        }
        close OO;close II;
    }

}	##GetPntE

############################################################################
sub GetPntT		#09/19/2007 11:32
############################################################################
 {
    my ($Par, $PntNum, $MatterName) = @_;
    GetPntE($Par, $PntNum);
	open(IN,"<$Par->{ClcName}.$PntNum")		or die "Could not open file $Par->{ClcName}.$PntNum : $!\n";
	my $NumPnt=0;
	while(<IN>){$NumPnt++;}
	my ($IntPar, $ReaderIntPar) = ("", "");
	for(my $i = 0;$i<$Par->{ClcTime}{NumIntPar};$i++){
		$IntPar = "$IntPar IntPar$i";
		$ReaderIntPar = "$ReaderIntPar Reader.IntPar$i";
	}
	my ($conf_name) = ("urs.cfg");
	open(OUT,">$conf_name");
	print OUT <<EOF
 URS_Curve {
    Variables {
        Mat       EOS_Savable         { $MatterName }
        IsoE      UrsCurve_Caloric    { NameDenc Reader.Denc NameEnergy Reader.Ener  NameMatter Mat }

        Reader    UrsCurve_FileReader { FileName $Par->{ClcName}.$PntNum
			DataNames {  Time Pos Pres Denc Ener Vel $IntPar } DefaultValue  0
		}
    }

    Output      URS_Curve::Output {
        FileToStore $Par->{ClcName}.$PntNum.t  VectorOfNames {
             Reader.Time  Reader.Pos Reader.Pres Reader.Denc IsoE.Temperature Reader.Ener IsoE.Sound Reader.Vel $ReaderIntPar
        }
    }
NumIter  $NumPnt    }

EOF
	;system("urs_curve.exe $conf_name");
#	unlink("$Par->{ClcName}.$PntNum");
}	##GetPntT

############################################################################
sub GetPntTInt		#09/19/2007 11:32
############################################################################
 {
    my ($Par, $PntNum, $MatterName, $IntNames) = @_;
    GetPntE($Par, $PntNum);
	open(IN,"<$Par->{ClcName}.$PntNum")		or die "Could not open file $Par->{ClcName}.$PntNum : $!\n";
	my $NumPnt=0;
	while(<IN>){$NumPnt++;}
	my ($IntPar, $ReaderIntPar, $IntMap) = ("", "", "" );
	for(my $i = 0;$i<$Par->{ClcTime}{NumIntPar};$i++){
		$IntPar = "$IntPar IntPar$i";
		$ReaderIntPar = "$ReaderIntPar Reader.IntPar$i";
		$IntMap = "$IntMap Reader.IntPar$i $IntNames->[$i]"
	}
	my ($conf_name) = ("urs.cfg");
	open(OUT,">$conf_name");
	print OUT <<EOF
 URS_Curve {
    Variables {
        Mat       EOS_Savable         { $MatterName }
        IsoE      UrsCurve_InOut    { NameMatter Mat Input_Mat2Clc_Name ( Reader.Denc Dencity Reader.Ener  Energy  $IntMap ) }

        Reader    UrsCurve_FileReader { FileName $Par->{ClcName}.$PntNum
			DataNames {  Time Pos Pres Denc Ener Vel $IntPar } DefaultValue  0
		}
    }

    Output      URS_Curve::Output {
        FileToStore $Par->{ClcName}.$PntNum.t  VectorOfNames {
             Reader.Time  Reader.Pos Reader.Pres Reader.Denc IsoE.Temperature Reader.Ener IsoE.Sound Reader.Vel $ReaderIntPar
        }
    }
NumIter  $NumPnt    }

EOF
	;system("urs_curve.exe $conf_name");
#	unlink("$Par->{ClcName}.$PntNum");
}	##GetPntTInt

############################################################################
sub ClcPntNum		#09/19/2007 1:24
############################################################################
 {
	my ($Par, $Layer, $PntNum) = @_;
	$Layer--;
	my @ClcLayer = @{$Par->{ClcLayer}};
	die "Bad Layer ".($Layer+1).". Max Layer is ".int(@ClcLayer)."\n"	if ($Layer>=int(@ClcLayer) || $Layer<0);
	my $s = 0;
	for(my $it=0;$it<$Layer;$it++){
		$s += $ClcLayer[$it]{N};
	}
	$PntNum = $ClcLayer[$Layer]{N} + $PntNum		if ($PntNum<0);
	return ($s+$PntNum, $ClcLayer[$Layer]{M});

}	##ClcPntNum
############################################################################
sub GetAllPnt		#09/19/2007 1:17
############################################################################
 {
    my ($Par, $IntMatLayer, $IntMatName, $IntNames) = @_;
	my @PntE = @{$Par->{ShowE}};
	my @PntT = @{$Par->{ShowT}};
	for(my $it=0;$it<int(@PntE);$it+=2){
		my @NumMatter = ClcPntNum($Par, $PntE[$it], $PntE[$it+1]);
        GetPntE($Par, $NumMatter[0]);
	}
	for(my $it=0;$it<int(@PntT);$it+=2){
		my @NumMatter = ClcPntNum($Par, $PntT[$it], $PntT[$it+1]);
		if ( !defined($IntMatLayer) || $IntMatLayer!=$PntT[$it] ){
            GetPntT($Par, $NumMatter[0], $NumMatter[1]);
		} else {
            GetPntTInt($Par, $NumMatter[0], $IntMatName, $IntNames);
		}
	}
}	##GetAllPnt

############################################################################
sub PackParams		#09/19/2007 12:37
############################################################################
 {
    my ($ClcName, $ShowE, $ShowT, $ClcLayer, $ClcTime, $TimeCoef) = @_;
	my %Par;
	$Par{ClcName} = $ClcName;
	$Par{ClcLayer} = $ClcLayer;
	$Par{ShowE} = $ShowE;
	$Par{ShowT} = $ShowT;
	$Par{ClcTime} = $ClcTime;
	$Par{ClcTime}{NumIntPar} = 0			if (!defined($Par{ClcTime}{NumIntPar}));
    $Par{TimeCoef} = $TimeCoef              if (defined($TimeCoef));
	return \%Par;
}	##PackParams

############################################################################
sub PrintHash		#09/18/2007 7:04
############################################################################
 {
	my ($h) = @_;
	my $Res="";
	for my $key (sort keys(%{$h}) ) {
		my $val = $h->{$key};
#	while ( my ($key, $val) = each(%{$h}) ) {
		$Res = $Res."<$key> => <$val>\n";
	}
	return $Res;
}	##PrintHash

sub IsNumber {
        return 0                if (!defined($_[0]));
        $_[0] =~ /^([+-]?)(?=\d|\.\d)\d*(\.\d*)?([Ee]([+-]?\d+))?$/;
}
 sub max {
      my $max = pop(@_);
      foreach my $foo (@_) {
          $max = $foo if IsNumber($foo) && $max < $foo;
      }
      $max;
  }
 sub min {
      my $min = pop(@_);
      foreach my $foo (@_) {
          $min = $foo if IsNumber($foo) && $min > $foo;
      }
      $min;
  }


############################################################################
sub SolveDenc  #09/18/2007 6:38
############################################################################
 {
# $Mat = FreeEIdeal   {  Material_File material.cfg Substance  FreeEIdealHe   }
 my ($Phe, $Mat, $Temp, $NumPnt) = @_;
 $Temp = 290                            if (!defined($Temp));
 $NumPnt = 1000                         if (!defined($NumPnt));
 open(OUT,">urs.cfg");
# my $NumPnt = 1000;
 print OUT <<EOF

 URS_Curve {
    Variables {

        Mat       EOS_Savable {   MatterFreeE {  $Mat  }   }
        IsoT      UrsCurve_FreeE { NameDenc Denc NameTemp Temp  NameMatter Mat }

         Denc           UrsCurve_StepClc { MinVal 1.e-5 MaxVal 1.  NumDivStp  $NumPnt  LogScale 1 NumSame 1  Centered 0  }
         Temp           UrsCurve_StepClc { MinVal $Temp MaxVal $Temp NumDivStp  1  LogScale 0 NumSame 1  Centered 0   }

    }

    Output      URS_Curve::Output {
        FileToStore urs_he.dat  VectorOfNames {
    IsoT.Pressure  Denc  IsoT.Energy
        }
    }
NumIter  $NumPnt    }

#        Mat       EOS_Savable {   MatterFreeE {  FreeEIdeal   {  Material_File material.cfg Substance  FreeEIdealHe   }   }   }

EOF
 ;open(OUT,">urs_he.val");
 print OUT <<EOF
$Phe
EOF
 ;system("urs_curve.exe urs.cfg");
 system("set1grph.exe urs_he.val urs_he.dat urs_he.res /i /a >tmp.tmp");
 open(IN,"<urs_he.res");
 my $l = <IN>;
 my @l = split(" ",$l);
# $Par->{DenHe} = $l[1];
# $Par->{EneHe} = $l[2];
 return ($l[1], $l[2]);
} ##SolveHeDenc


#		Matter NumPnt Length Vel  Denc Ener Pres
#my $ClcLayer = [
#	{X=>1,M=>"MatterABu  { Material_File material.cfg Substance Steel-Tern  }",
#		N=>100, L=>1.51, V=>5.335 , D=>0, E=>0, P=>0},
#	{X=>2,M=>"MatterABu  { Material_File material.cfg Substance Steel-Tern  }",
#		N=>100, L=>1.51, V=>0 , D=>0, E=>0, P=>0},
#	{X=>3,M=>"MatterSpl  { he_ros_e.spl  }",
#		N=>100, L=>6.51, V=>0 , D=>1e-3, E=>0.1, P=>0},
#	{X=>4,M=>"MatterABu  { Material_File material.cfg Substance Z-sapphir  }",
#		N=>100, L=>5.51, V=>0 , D=>0, E=>0, P=>0}
#	];

#my $ClcTime = { TimeStp=> 1e-3, TimeWrite=> 5, EndTime=> 2000, TimeStability=>0.01, NumIntPar=>0 };

#my $ShowE = [
#	4, 5 ,
#	4,-5 ,
#	2,-5
#	];
#my $ShowT = [
#	3, 5 ,
#	3,-5 ,
#	3,50
#	];


#my $Par = PackParams("isent", $ShowE, $ShowT, $ClcLayer, $ClcTime);
#ClcAssembly($Par);
#GetAllPnt($Par);


1;





