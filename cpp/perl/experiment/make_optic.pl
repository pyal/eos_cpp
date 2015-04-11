use strict;use warnings;
my $MakeTbr = 1;

require ClcGasd;
require AnalyzeData;
############################################################################
sub GetNameExt		#10/22/2007 8:27
############################################################################
 {
	my ($EkspPar, $namefull) = @_;
	my @name = split(/\./,$namefull);
	my $ext = pop(@name);
	my $add = $EkspPar->{SrcDir};
	$add = substr($add,0,length($add)-2);
    my $name_src = join("\.",@name);
    my $name_clc = $add.".".join("\.",@name);
#    $name_clc = $name_src;
    return ($name_src, $name_clc, $ext);

}	##GetNameExt
############################################################################
sub NormalizeName		#10/22/2007 5:26
############################################################################
 {
	my ($EkspPar) = @_;
	for my $work ("CalibFile", "ExpFile") {
        my ($name_src, $name_clc, $ext) = GetNameExt($EkspPar, $EkspPar->{$work});
        $EkspPar->{$work} = $name_clc;
	}
}	##NormalizeName
############################################################################
sub NormalizeInput		#10/22/2007 12:11
############################################################################
 {
	my ($EkspPar) = @_;
	for my $work ("CalibFile", "ExpFile") {
        my ($name_src, $name_clc, $ext) = GetNameExt($EkspPar, $EkspPar->{$work});
#print $EkspPar->{$work},"\n";
#print $name;
#die;
#       $EkspPar->{$work} = $name; ---------------------------------------
        $name_clc = $EkspPar->{ClcDir}.$name_clc;
        $name_src = $EkspPar->{SrcDir}.$name_src;
        system("ConvertTekWfm.exe $name_src.$ext /CSV $name_src.csv")       if ( lc($ext) eq "wfm" );
		die "do not know how to work with extension $ext\n"			    if ( lc($ext) ne "wfm"  && lc($ext) ne "csv");
        open(IN1, "< $name_src.csv")            or die "Could not open input file $name_src.csv:$!\n";
        open(OUT1, "> $name_clc.dat")       or die "Could not open output file $name_clc.dat:$!\n";
		while(<IN1>){
			my @l = split(",",$_);
			my $l = join(" ",@l);
			@l = split(" ",$l);
			next			if (!ClcGasd::IsNumber($l[0]));
			$l[0] *= 1e9;
			my $n = "\x0D\x0A";
			print OUT1 join("\t ",@l),$n;
		}
		close IN1;close OUT1;
        unlink("$name_src.csv")                 if ( lc($ext) eq "wfm" );
	}


}	##NormalizeInput

############################################################################
sub ShiftTime     #10/21/2007 11:22
############################################################################
 {
    my ($name, $shiftT, $nameout) = @_;
    open IN1, "< $name"            or die "Could not open input file $name:$!\n";
    open OUT1, "> $nameout"            or die "Could not open output file $nameout:$!\n";
    while(<IN1>){
        my @l = split;
        print OUT1 ($_), next        if (!ClcGasd::IsNumber($l[0]));
        $l[0]+=$shiftT;
        print OUT1  join(" ",@l), "\n";
    }
    close IN1;close OUT1;

}   ##ShiftTime


############################################################################
sub ClcCalibration		#10/22/2007 1:10
############################################################################
 {
	my ($name) = @_;
	my ($dataall, $data, $stat);
	$dataall = AnalyzeData::StoreArray($name);
	$data = $dataall->[1];
#	$stat = AnalyzeData::GatherArrayStat($data);
	my ($statL, $statU) = (AnalyzeData::GatherArrayStat($data,0,int(@{$data})*0.2), AnalyzeData::GatherArrayStat($data,int(@{$data})*0.8, int(@{$data})) );

	my ($LookUp, $LimVal, $LineN) = (1, 0, 0);
#    $LimVal = AnalyzeData::min( 3*abs($stat->{Mean}), 0.6*AnalyzeData::max(abs($stat->{Max}),abs($stat->{Min})));
#	$LimVal = 2*($stat->{Mean});
#	$LimVal *= 0.5;
#	$stat->{LimVal} = $LimVal;
#	my $Pnt1 = AnalyzeData::LookStableDrop($data, 0, -abs($LimVal),abs($LimVal), 20,5);
#	my $Pnt2 = AnalyzeData::LookStableDrop($data, $Pnt1+20, AnalyzeData::min(0.5*$LimVal, 10*$LimVal), AnalyzeData::max(0.5*$LimVal, 10*$LimVal), 20,5);

	my ($Vl, $Vu) = ($statL->{Mean}, $statU->{Mean});
	my $Mis = abs($Vl-$Vu)/1.5;
	my $Pnt1 = AnalyzeData::LookStableDrop($data, 0, $Vl-$Mis,$Vl+$Mis, 20,5);
	my $Pnt2 = AnalyzeData::LookStableDrop($data, $Pnt1+20, $Vu-$Mis,$Vu+$Mis, 20,5);

print "Found Lims 0 $Pnt1 $Pnt2 ",int(@{$data}),"\n";
#print "Mean $LimVal, lowbnd $LimVal upbnd ",10*$LimVal,"\n";
#die;
    my ($low1, $low2) = AnalyzeData::FindStableRegion($data,0,$Pnt1, 1.8, 20);
    my ($up1, $up2)   = AnalyzeData::FindStableRegion($data,$Pnt1+20, $Pnt2, 1.8, 20);
	$stat = AnalyzeData::GatherArrayStat($data, $low1, $low2);
	my $lowv = $stat->{Mean};
	$stat = AnalyzeData::GatherArrayStat($data, $up1, $up2);
	my $upv = $stat->{Mean};
	my ($lowt1, $lowt2) = ($dataall->[0][$low1], $dataall->[0][$low2]);
	my ($upt1, $upt2) = ($dataall->[0][$up1], $dataall->[0][$up2]);

	open OUT1, "> $name.cal.stat"		or die "Could not open output file $name.cal.stat:$!\n";
	my $DataLength = int(@{$data});
	print OUT1  <<EOF
LowVal  $lowv
StartPnt $low1  FinalPnt $low2
StartVal $lowt1  FinalVal $lowt2
UpVal  $upv
StartPnt $up1  FinalPnt $up2
StartVal $upt1  FinalVal $upt2
Found Lims 0 ->   $Pnt1 $Pnt2  <-  $DataLength
LowLimEst - $Vl   UpLimEst $Vu
EOF
	;
	close OUT1;
	return abs($upv-$lowv);

}	##ClcCalibration
############################################################################
sub ClcElectra		#10/22/2007 12:00
############################################################################
 {
	my ($EkspPar) = @_;
	my $dir = $EkspPar->{ClcDir};
	ShiftTime("$dir$EkspPar->{ExpFile}.dat", -$EkspPar->{Shift}, "$dir$EkspPar->{ExpFile}.cond");
#	unlink("$dir$EkspPar->{ExpFile}.dat");
#	unlink("$dir$EkspPar->{CalibFile}.dat");

}	##ClcElectra

############################################################################
sub ClcOptics		#10/22/2007 12:32
############################################################################
 {
	my ($EkspPar) = @_;
	my $dir = $EkspPar->{ClcDir};
	my $Calib ;
	if (defined($EkspPar->{Calibration}) && AnalyzeData::IsNumber($EkspPar->{Calibration}) && $EkspPar->{Calibration}>0) {
		$Calib = $EkspPar->{Calibration};
	}else {
		$Calib = ClcCalibration("$dir$EkspPar->{CalibFile}.dat");
	}
	open OUT1, "> $dir$EkspPar->{ExpFile}.da"		or die "Could not open out file $dir$EkspPar->{ExpFile}.da:$!\n";
	my ($Lam, $UseLine) = ( $EkspPar->{Lambda}, "\"\"");
	if ( lc($EkspPar->{Lambda}) eq "si" ) {
		$Lam = 700;
		$UseLine = "N";
	}
	if ( lc($EkspPar->{Lambda}) eq "dfd" ) {
		$Lam = 1500;
		$UseLine = "N";
	}

    my $n = "\x0D\x0A";
    print OUT1 "1 $n$Lam $n$UseLine $n$Calib $n$EkspPar->{Opacity}$n";
	open IN1, "< $dir$EkspPar->{ExpFile}.dat"		or die "Could not open input file $dir$EkspPar->{ExpFile}.dat:$!\n";
	while(<IN1>){
		my @l = split;
		$l[0] -= $EkspPar->{Shift};
        print OUT1 join("    ",@l),"$n";
	}
	if ($MakeTbr){
        system("mv $dir$EkspPar->{ExpFile}.da tmp.dat");

		if ($Lam<=1080){
            system("vol2temp tmp.dat tmp.tbr");
		} else {
            system("vol2temp tmp.dat /Rdfd1000.dat tmp.tbr");
		}
        system("mv  tmp.tbr $dir$EkspPar->{ExpFile}.tbr");
        system("mv  tmp.dat $dir$EkspPar->{ExpFile}.da");
	}
#	unlink("$dir$EkspPar->{ExpFile}.da");
#	unlink("$dir$EkspPar->{ExpFile}.dat");
#	unlink("$dir$EkspPar->{CalibFile}.dat");

}	##ClcOptics


############################################################################
sub GenerateResult		#10/22/2007 12:32
############################################################################
 {
	my ($FullTable) = @_;
    my (%tbr, %cond);
    my $dir;
    while ( my ($par_file, $item ) = each(%{$FullTable}) ){
        while ( my ($num, $EkspPar ) = each(%{$item}) ){
            print $EkspPar->{ExpFile},"\n";
            $dir = $EkspPar->{ClcDir};
            if (ClcGasd::IsNumber( $EkspPar->{Lambda}) || lc($EkspPar->{Lambda}) eq "si" || lc($EkspPar->{Lambda}) eq "dfd"){
                $tbr{$EkspPar->{Lambda}} = "$EkspPar->{ExpFile}.tbr";
            } else {
                $cond{$EkspPar->{Lambda}} = "$EkspPar->{ExpFile}.cond";
            }
        }
    }

    my ($Res, $Str, $Sum) = ("", "", "");
	my $n = "\x0D\x0A";
    for my $key (sort {$a<=>$b} keys(%tbr) ) {
        my $val = $tbr{$key};
        $Res = $Res."$key ";
        $Str = $Str."$val=>$key$n";
		$Sum = $Sum."$dir$val ";
    }
#    system("set1grph $Sum ".$dir."sum.tbr /i /a");
    AnalyzeData::SumSortedFiles($Sum, $dir."sum.tbr","time_ns $Res$n");
    open OUT1, "> ".$dir."sum.tbr.desc"         or die "Could not open out file $dir"."sum.tbr.desc:$!\n";
    print OUT1 "$Res$n$Str$n";
    return      if (!defined($cond{R}));
    my $sum = "$dir$cond{Up}  $dir$cond{Um} $dir$cond{R} ";
    AnalyzeData::SumSortedFiles($sum, $dir."sum.cond");
#    system("set1grph $sum ".$dir."sum.cond /i /a");
}	##GenerateResult



my @tables = split(" ", $ARGV[0]);
my ($LowLim, $UpLim) = ($ARGV[1], $ARGV[2]);
if ( defined($UpLim) && AnalyzeData::IsNumber($UpLim) && AnalyzeData::IsNumber($LowLim)) {
	AnalyzeData::ShortenData("clc/sum.tbr", "clc/sum.sh.tbr", $LowLim, $UpLim);
	AnalyzeData::ShortenData("clc/sum.cond", "clc/sum.sh.cond", $LowLim, $UpLim);
	die;
}

system("mkdir clc");
my $FullTable = {};
for ( my $i = 0;$i<1000;$i++ ) {
	last		if (!defined($tables[$i]));
	my $par_file = $tables[$i];
	open(IN, " <$par_file ")			or die "Could not open file $par_file: $!\n";
	my @srcdir = split(/\./,$par_file);pop(@srcdir);
	my $srcdir = join("\.",@srcdir)."/";
    my $n = 0;
	<IN>;
	while(<IN>){
		my $EkspPar = {ClcDir=>"clc/", SrcDir=>$srcdir};
#  Cal         Exp        Lambda  Shift   Opacity
		( $EkspPar->{CalibFile}, $EkspPar->{ExpFile}, $EkspPar->{Lambda}, $EkspPar->{Shift}, $EkspPar->{Opacity}, $EkspPar->{Calibration} ) = split;
		next						if (!defined($EkspPar->{Opacity}));
#        NormalizeInput($EkspPar);
		NormalizeName($EkspPar);
		if (ClcGasd::IsNumber( $EkspPar->{Lambda}) || lc($EkspPar->{Lambda}) eq "si" || lc($EkspPar->{Lambda}) eq "dfd"){
           ClcOptics($EkspPar);
		} else {
#            ClcElectra($EkspPar);
		}
        $FullTable->{$par_file}{$n++} = $EkspPar;
	}
}
GenerateResult($FullTable);
