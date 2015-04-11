#!/usr/local/bin/perl5.6.1
use strict;
use warnings;
no strict "refs";
$/="\x0D\x0A";

#require test_urs_generator;

# Read initial configuration parameters - result %Conf
sub ReadConf{
    my ($ConfName) = @_;
    my %Conf;
    open (IN,"< $ConfName")     or die "Could not read file $ConfName; Error $!\n";
    while(<IN>){
        chomp;
        my @l = split " ",$_;
        next                if (int(@l)==0 || substr($l[0],0,2) eq "//");
        my (@tmp, $tmp);
        if (int(@l)==1){
            @tmp = split(" ",<IN>);
            $Conf{$l[0]} = join(" ",@tmp);
        } else {
            @tmp = @l;
            $tmp = shift(@tmp);
            $Conf{$tmp} = join(" ",@tmp);
        }

    }
    close(IN);
    return %Conf;
}

my $StreamOut = *STDOUT{IO};
sub Hugoniot_Denc{
    my ($cfg) = @_;
    my $NameDencHug="Dencity";
    $NameDencHug="Reader.".$cfg->{'$GetFromFileColumn'} if ($cfg->{'$ReadFile'}==1);
    my $FileInPar="";
    for (my $i=1;$i<=$cfg->{'$GetFromFileNumColumn'};$i++){
        $FileInPar = $FileInPar.$i." ";
    }
    my $FileOutPar;
    my %Subst=(
        "R","Hug.Dencity",
        "P","Hug.Pressure",
        "E","Hug.Energy",
        "U","Hug.Velocity",
        "T","EOSHug.Temperature",
        "S","EOSHug.Sound",
        "D","Hug.ShockVelocity",
    );
    my @code = split(" ",$cfg->{'$OutputPar'});
    for (my $i=0;$i<int(@code);$i++){
        if (defined($Subst{$code[$i]})){
            $FileOutPar = "$FileOutPar $Subst{$code[$i]}";
        } else {
            $FileOutPar = "$FileOutPar $code[$i]";
        }
    }
    print $StreamOut "
 URS_Curve {
    Variables {
        Matter      EOS_Savable { $cfg->{'$Matter'} }
        Dencity     UrsCurve_StepClc { MinVal $cfg->{'$SDenc'} MaxVal $cfg->{'$FDenc'} NumDivStp $cfg->{'$NumClcPnt'} LogScale $cfg->{'$LogScale'} NumSame 1 Centered 0 }
        Reader      UrsCurve_FileReader { FileName $cfg->{'$GetFromFile'} DataNames {  $FileInPar  } DefaultValue  0 }

        Hug         UrsCurve_EOS_Hugoniot { NameDenc $NameDencHug NameMatter Matter StartDenc $cfg->{'$SDenc'} StartEner $cfg->{'$SEner'} StartPres $cfg->{'$HugStartPres'} StartVel 0 PressureDependece 0 RecalcPres  $cfg->{'$HugRecalcHugP'} }
        EOSHug      UrsCurve_Caloric { NameDenc Hug.Dencity NameEnergy Hug.Energy NameMatter Matter }
        $cfg->{'$TestClass'}
    }

    Output          URS_Curve::Output {
        FileToStore $cfg->{'$OutFile'} VectorOfNames {
            $FileOutPar $cfg->{'$TestVar'}
        }
    }
    NumIter  $cfg->{'$NumClcPnt'}
}
";

}

sub Hugoniot_Pres{
    my ($cfg) = @_;
    my $NameDencHug="Pressure";
    $NameDencHug="Reader.".$cfg->{'$GetFromFileColumn'} if ($cfg->{'$ReadFile'}==1);
    my $FileInPar="";
    for (my $i = 1;$i<=$cfg->{'$GetFromFileNumColumn'};$i++){
        $FileInPar = $FileInPar.$i." ";
    }
    my $FileOutPar="";
    my %Subst=(
        "R","Hug.Dencity",
        "P","Hug.Pressure",
        "E","Hug.Energy",
        "U","Hug.Velocity",
        "T","EOSHug.Temperature",
        "S","EOSHug.Sound",
        "D","Hug.ShockVelocity",
    );
    my @code = split(" ",$cfg->{'$OutputPar'});
    for (my $i=0;$i<int(@code);$i++){
        if (defined($Subst{$code[$i]})){
            $FileOutPar = "$FileOutPar $Subst{$code[$i]}";
        } else {
            $FileOutPar = "$FileOutPar $code[$i]";
        }
    }
    print $StreamOut "
 URS_Curve {
    Variables {
        Matter    EOS_Savable { $cfg->{'$Matter'} }
        Pressure  UrsCurve_StepClc { MinVal $cfg->{'$SPres'} MaxVal $cfg->{'$FPres'} NumDivStp $cfg->{'$NumClcPnt'} LogScale $cfg->{'$LogScale'} NumSame 1 Centered 0 }
        Reader    UrsCurve_FileReader { FileName $cfg->{'$GetFromFile'} DataNames {  $FileInPar  } DefaultValue  0 }

        Hug       UrsCurve_EOS_Hugoniot { NameDenc $NameDencHug NameMatter Matter StartDenc $cfg->{'$SDenc'} StartEner $cfg->{'$SEner'} StartPres $cfg->{'$HugStartPres'} StartVel 0 PressureDependece 1 RecalcPres  $cfg->{'$HugRecalcHugP'} }
        EOSHug    UrsCurve_Caloric { NameDenc Hug.Dencity NameEnergy Hug.Energy NameMatter Matter }
        $cfg->{'$TestClass'}

    }

    Output    URS_Curve::Output {
        FileToStore $cfg->{'$OutFile'} VectorOfNames {
            $FileOutPar $cfg->{'$TestVar'}
        }
    }
    NumIter  $cfg->{'$NumClcPnt'}
}
";

}

# ==========================================================================================
# =================================    IsoEntrope      =====================================
# ==========================================================================================
sub IsoEntrope {
    my ($cfg) = @_;
    my $NameDencHug="Dencity";
    $NameDencHug="Reader.".$cfg->{'$GetFromFileColumn'} if ($cfg->{'$ReadFile'}==1);
    my $FileInPar="";
    for (my $i=1;$i<=$cfg->{'$GetFromFileNumColumn'};$i++){
        $FileInPar = $FileInPar.$i." ";
    }
    my $FileOutPar;
    my %Subst=(
        "R",$NameDencHug,
        "P","EOSIse.Pressure",
        "E","Ise.Energy",
        "T","EOSIse.Temperature",
        "S","EOSIse.Sound",
        "U","Ise.Velocity"
    );
    my @code = split(" ",$cfg->{'$OutputPar'});
    for (my $i=0;$i<int(@code);$i++){
        if (defined($Subst{$code[$i]})){
            $FileOutPar = "$FileOutPar $Subst{$code[$i]}";
        } else {
            $FileOutPar = "$FileOutPar $code[$i]";
        }
    }
    print $StreamOut "
URS_Curve {
    Variables {
        Matter  EOS_Savable { $cfg->{'$Matter'} }
        Dencity UrsCurve_StepClc { MinVal $cfg->{'$SDenc'} MaxVal $cfg->{'$FDenc'} NumDivStp $cfg->{'$NumClcPnt'} LogScale $cfg->{'$LogScale'} NumSame 1 Centered 0 }
        Reader  UrsCurve_FileReader { FileName $cfg->{'$GetFromFile'} DataNames {  $FileInPar  } DefaultValue  0 }

        Ise     UrsCurve_EOS_Isoentrope { NameDenc $NameDencHug NameMatter Matter StartE $cfg->{'$SEner'} StartU 0 ClcU $cfg->{'$ClcIsentU'} }
        EOSIse  UrsCurve_Caloric { NameDenc $NameDencHug NameEnergy Ise.Energy NameMatter Matter }
        $cfg->{'$TestClass'}
    }

    Output     URS_Curve::Output {
        FileToStore $cfg->{'$OutFile'} VectorOfNames {
            $FileOutPar $cfg->{'$TestVar'}
        }
    }
    NumIter  $cfg->{'$NumClcPnt'}
}
";

}

# ==========================================================================================
# =================================     FreeE_IsoTherm         =====================================
# ==========================================================================================
sub FreeE_IsoTherm {
    my ($cfg) = @_;
    my $NameDencHug="Dencity";
    $NameDencHug="Reader.".$cfg->{'$GetFromFileColumn'} if ($cfg->{'$ReadFile'}==1);
    my $NameTemperature = "Temperature";
    $NameTemperature ="Reader.".$cfg->{'$GetFromFile2Column'}   if ($cfg->{'$ReadFile'}==1);
    my $FileInPar="";
    for (my $i=1;$i<=$cfg->{'$GetFromFileNumColumn'};$i++){
        $FileInPar = $FileInPar.$i." ";
    }
    my $FileOutPar;
    my %Subst=(
        "R",$NameDencHug,
        "P","IsoT.Pressure",
        "E","IsoT.Energy",
        "T",$NameTemperature,
        "S","IsoT.Sound",
        "F","IsoT.FreeE",
        "G","IsoT.FreeG"
    );
    my $NumIter = $cfg->{'$NumClcPnt'}*$cfg->{'$NumClc2Pnt'};
    my @code = split(" ",$cfg->{'$OutputPar'});
    for (my $i=0;$i<int(@code);$i++){
        if (defined($Subst{$code[$i]})){
            $FileOutPar = "$FileOutPar $Subst{$code[$i]}";
        } else {
            $FileOutPar = "$FileOutPar $code[$i]";
        }
    }
    print $StreamOut "
URS_Curve {
    Variables {
        Matter       EOS_Savable { $cfg->{'$Matter'} }
        Dencity      UrsCurve_StepClc { MinVal $cfg->{'$SDenc'} MaxVal $cfg->{'$FDenc'} NumDivStp $cfg->{'$NumClcPnt'} LogScale $cfg->{'$LogScale'} NumSame 1 Centered 0 }
        Reader       UrsCurve_FileReader { FileName $cfg->{'$GetFromFile'} DataNames {  $FileInPar  } DefaultValue  0 }
        Temperature  UrsCurve_StepClc { MinVal $cfg->{'$STemp'} MaxVal $cfg->{'$FTemp'} NumDivStp $cfg->{'$NumClc2Pnt'} LogScale $cfg->{'$Log2Scale'} NumSame $cfg->{'$NumClcPnt'} Centered 0 }
        IsoT         UrsCurve_FreeE { NameDenc $NameDencHug NameTemp $NameTemperature NameMatter Matter }
        $cfg->{'$TestClass'}
    }

    Output     URS_Curve::Output {
        FileToStore $cfg->{'$OutFile'} VectorOfNames {
            $FileOutPar $cfg->{'$TestVar'}
        }
    }
    NumIter  $NumIter
}
";

}


# ==========================================================================================
# =================================     IsoEnergy       ====================================
# ==========================================================================================
sub IsoEnergy {
    my ($cfg) = @_;
    my $NameDencHug="Dencity";
    $NameDencHug="Reader.".$cfg->{'$GetFromFileColumn'} if ($cfg->{'$ReadFile'}==1);
    my $NameEnergy = "Energy";
    $NameEnergy ="Reader.".$cfg->{'$GetFromFile2Column'}    if ($cfg->{'$ReadFile'}==1);
    my $FileInPar="";
    for (my $i=1;$i<=$cfg->{'$GetFromFileNumColumn'};$i++){
        $FileInPar = $FileInPar.$i." ";
    }
    my $FileOutPar;
    my %Subst=(
        "R",$NameDencHug,
        "P","EOSIsoE.Pressure",
        "E",$NameEnergy,
        "T","EOSIsoE.Temperature",
        "S","EOSIsoE.Sound"
    );
    my @code = split(" ",$cfg->{'$OutputPar'});
    for (my $i=0;$i<int(@code);$i++){
        if (defined($Subst{$code[$i]})){
            $FileOutPar = "$FileOutPar $Subst{$code[$i]}";
        } else {
            $FileOutPar = "$FileOutPar $code[$i]";
        }
    }
    my $NumIter = $cfg->{'$NumClcPnt'}*$cfg->{'$NumClc2Pnt'};
    print $StreamOut "
URS_Curve {
    Variables {
        Matter  EOS_Savable { $cfg->{'$Matter'} }
        Dencity UrsCurve_StepClc { MinVal $cfg->{'$SDenc'} MaxVal $cfg->{'$FDenc'} NumDivStp $cfg->{'$NumClcPnt'} LogScale $cfg->{'$LogScale'} NumSame 1 Centered 0 }
        Energy  UrsCurve_StepClc { MinVal $cfg->{'$SEner'} MaxVal $cfg->{'$FEner'} NumDivStp $cfg->{'$NumClc2Pnt'} LogScale $cfg->{'$Log2Scale'} NumSame $cfg->{'$NumClcPnt'} Centered 0 }
        Reader  UrsCurve_FileReader { FileName $cfg->{'$GetFromFile'} DataNames {  $FileInPar  } DefaultValue  0 }

        EOSIsoE UrsCurve_Caloric { NameDenc $NameDencHug NameEnergy $NameEnergy NameMatter Matter }
        $cfg->{'$TestClass'}
    }

    Output   URS_Curve::Output {
        FileToStore $cfg->{'$OutFile'} VectorOfNames {
            $FileOutPar $cfg->{'$TestVar'}
        }
    }
    NumIter  $NumIter
}
";

}

# ==========================================================================================
# =================================     FindTempE       ====================================
# ==========================================================================================
sub FindTempE {
    my ($cfg) = @_;
    my $NameDencHug="Dencity";
    $NameDencHug="Reader.".$cfg->{'$GetFromFileColumn'} if ($cfg->{'$ReadFile'}==1);
    my $NameTemperature = "Temperature";
    $NameTemperature ="Reader.".$cfg->{'$GetFromFile2Column'}   if ($cfg->{'$ReadFile'}==1);
    my $FileInPar="";
    for (my $i=1;$i<=$cfg->{'$GetFromFileNumColumn'};$i++){
        $FileInPar = $FileInPar.$i." ";
    }
    my $FileOutPar;
    my %Subst=(
        "R",$NameDencHug,
        "P","EOSIsoE.Pressure",
        "E","FindE",
        "T","EOSIsoE.Temperature",
        "S","EOSIsoE.Sound"
    );
    my @code = split(" ",$cfg->{'$OutputPar'});
    for (my $i=0;$i<int(@code);$i++){
        if (defined($Subst{$code[$i]})){
            $FileOutPar = "$FileOutPar $Subst{$code[$i]}";
        } else {
            $FileOutPar = "$FileOutPar $code[$i]";
        }
    }
    my $NumIter = $cfg->{'$NumClcPnt'}*$cfg->{'$NumClc2Pnt'};
    print $StreamOut "
URS_Curve {
    Variables {
        Matter      EOS_Savable { $cfg->{'$Matter'} }
        Dencity     UrsCurve_StepClc { MinVal $cfg->{'$SDenc'} MaxVal $cfg->{'$FDenc'} NumDivStp $cfg->{'$NumClcPnt'} LogScale $cfg->{'$LogScale'} NumSame 1 Centered 0 }
        Temperature UrsCurve_StepClc { MinVal $cfg->{'$STemp'} MaxVal $cfg->{'$FTemp'} NumDivStp $cfg->{'$NumClc2Pnt'} LogScale $cfg->{'$Log2Scale'} NumSame $cfg->{'$NumClcPnt'} Centered 0 }
        Reader      UrsCurve_FileReader { FileName $cfg->{'$GetFromFile'} DataNames {  $FileInPar  } DefaultValue  0 }

        FindE       UrsCurve_EOS_FindE { NameDenc $NameDencHug NameTemp $NameTemperature NameMatter Matter }

        EOSIsoE     UrsCurve_Caloric { NameDenc $NameDencHug NameEnergy FindE NameMatter Matter }
        $cfg->{'$TestClass'}
    }

    Output   URS_Curve::Output {
        FileToStore $cfg->{'$OutFile'} VectorOfNames {
            $FileOutPar $cfg->{'$TestVar'}
        }
    }
    NumIter  $NumIter }
}
";

}


# ==========================================================================================
# =================================     SplineFreeConstr    ================================
# ==========================================================================================
sub SplineFreeConstr {
    my ($cfg) = @_;
    my $NameDencHug="Dencity";
    $NameDencHug="Reader.".$cfg->{'$GetFromFileColumn'} if ($cfg->{'$ReadFile'}==1);
    my $NameTemperature = "Temperature";
    $NameTemperature ="Reader.".$cfg->{'$GetFromFile2Column'}   if ($cfg->{'$ReadFile'}==1);
    my $NameFreeE = "UrsFreeE.FreeE";
    $NameFreeE ="Reader.".$cfg->{'$GetFromFile3Column'} if ($cfg->{'$ReadFile'}==1);
    my $FileInPar="";
    for (my $i=1;$i<=$cfg->{'$GetFromFileNumColumn'};$i++){
        $FileInPar = $FileInPar.$i." ";
    }
    my $FileOutPar = "";
    my %Subst=(
        "R",$NameDencHug,
        "P","UrsFreeE.Pressure",
        "E","UrsFreeE.Energy",
        "S","UrsFreeE.Sound",
        "T",$NameTemperature,
        "F","UrsFreeE.FreeE"
    );
    my $NumIter = $cfg->{'$NumClcPnt'}*$cfg->{'$NumClc2Pnt'};
    $cfg->{'$OutputPar'} = ""                   if (!defined($cfg->{'$OutputPar'}));
    my @code = split(" ",$cfg->{'$OutputPar'});
    for (my $i=0;$i<int(@code);$i++){
        if (defined($Subst{$code[$i]})){
            $FileOutPar = "$FileOutPar $Subst{$code[$i]}";
        } else {
            $FileOutPar = "$FileOutPar $code[$i]";
        }
    }

    print $StreamOut "
URS_Curve {
    Variables {
        Dencity     UrsCurve_StepClc { MinVal $cfg->{'$SDenc'} MaxVal $cfg->{'$FDenc'} NumDivStp $cfg->{'$NumClcPnt'} LogScale $cfg->{'$LogScale'} NumSame 1 Centered 0 }
        Temperature UrsCurve_StepClc { MinVal $cfg->{'$STemp'} MaxVal $cfg->{'$FTemp'} NumDivStp $cfg->{'$NumClc2Pnt'} LogScale $cfg->{'$Log2Scale'} NumSame $cfg->{'$NumClcPnt'} Centered 0 }
        Reader      UrsCurve_FileReader { FileName $cfg->{'$GetFromFile'} DataNames {  $FileInPar  } DefaultValue  0 }

        Matter      EOS_Savable { $cfg->{'$Matter'} }
        UrsFreeE    UrsCurve_FreeE { NameDenc $NameDencHug NameTemp $NameTemperature NameMatter Matter }

        SplineFree  UrsCurve_SplConstr { NameX  $NameTemperature NameY $NameDencHug NameZ $NameFreeE ResSplineName  $cfg->{'$Spl_FileName'} SplineDescription $cfg->{'$Spl_Description'} LogX 1 LogY 1 LogZ 1 AddBeforeLogX $cfg->{'$Spl_AddBeforeLogX'} AddBeforeLogY $cfg->{'$Spl_AddBeforeLogY'}  AddBeforeLogZ $cfg->{'$Spl_AddBeforeLogZ'} MulX $cfg->{'$Spl_MulX'} MulY $cfg->{'$Spl_MulY'} MulZ $cfg->{'$Spl_MulZ'} GenerationMisfit $cfg->{'$Spl_GenerationMisfit'}  GenerationNumX $cfg->{'$Spl_GenerationNumX'} GenerationNumY $cfg->{'$Spl_GenerationNumY'}  }
    }

    Output   URS_Curve::Output {
                FileToStore $cfg->{'$OutFile'} VectorOfNames {
                    SplineFree $FileOutPar
                }
    }
    NumIter  $NumIter
}

        MatterSpl  ReferenceClass_{ NOT_NULL    EOS_Savable { MatterFreeE MatterFreeSpl  $cfg->{'$Spl_FileName'} } }
        EOS_Spl     ReferenceClass_{  NOT_NULL UrsCurve_FreeE { NameDenc $NameDencHug NameTemp $NameTemperature NameMatter MatterSpl }   }

";

}


sub GenerateConfig{
    my ($cfg_name) = @_;
#   test_urs_generator::MakeStdConfig($cfg_name);
    my %Cfg = ReadConf($cfg_name);
#   while(my($key,$val)=each(%Cfg)){print "$key\n$val\n\n"; }
    my $ToDo = $Cfg{'$CurveType'};
    &$ToDo(\%Cfg);
}

#################################################################################
sub GenerateTestIsoT{
    my ($Cfg) = @_;
    my %Cfg = %{$Cfg};
    my $BaseOutName = $Cfg{'$OutFile'};

    $Cfg{'$ReadFile'} = 0;
    $Cfg{'$OutFile'} = "$BaseOutName.IsoT.test";
    $Cfg{'$OutputPar'} = " R P E T S ";

    open(OUT,">$BaseOutName.IsoT.cfg");$StreamOut = *OUT{IO};
    FreeE_IsoTherm(\%Cfg);
    close(OUT);
    system("urs_curve $BaseOutName.IsoT.cfg");

    $Cfg{'$TestClass'} = "TestClass UrsCurve_XY_Test { NameY1 Reader.2 NameY2 IsoT.Pressure MaxMeanError 1e-006 }   }   ";
    $Cfg{'$TestVar'} = " TestClass ";
    $Cfg{'$ReadFile'} = 1;
    $Cfg{'$GetFromFile'} = $Cfg{'$OutFile'};
    $Cfg{'$GetFromFileColumn'} = 1;
    $Cfg{'$GetFromFile2Column'} = 4;
    $Cfg{'$OutFile'} = "$BaseOutName.IsoT.dat";
    $Cfg{'$GetFromFileNumColumn'} = 5;

    open(OUT,">$BaseOutName.IsoT.cfg");$StreamOut = *OUT{IO};
    FreeE_IsoTherm(\%Cfg);
    close(OUT);

}
sub GenerateTestIsoT_Reduced{
    my ($Cfg) = @_;
    my %Cfg = %{$Cfg};
    my $BaseOutName = $Cfg{'$OutFile'};

    $Cfg{'$ReadFile'} = 0;
    $Cfg{'$OutFile'} = "$BaseOutName.IsoT.test";
    $Cfg{'$OutputPar'} = " R P T ";

    open(OUT,">$BaseOutName.IsoT.cfg");$StreamOut = *OUT{IO};
    FreeE_IsoTherm(\%Cfg);
    close(OUT);
    system("urs_curve $BaseOutName.IsoT.cfg");

    $Cfg{'$TestClass'} = "TestClass UrsCurve_XY_Test { NameY1 Reader.2 NameY2 IsoT.Pressure MaxMeanError 1e-006 }   ";
    $Cfg{'$TestVar'} = " TestClass ";
    $Cfg{'$ReadFile'} = 1;
    $Cfg{'$GetFromFile'} = $Cfg{'$OutFile'};
    $Cfg{'$GetFromFileColumn'} = 1;
    $Cfg{'$GetFromFile2Column'} = 3;
    $Cfg{'$OutFile'} = "$BaseOutName.IsoT.dat";
    $Cfg{'$GetFromFileNumColumn'} = 3;

    open(OUT,">$BaseOutName.IsoT.cfg");$StreamOut = *OUT{IO};
    FreeE_IsoTherm(\%Cfg);
    close(OUT);

}
sub GenerateTestIosE{
    my ($Cfg) = @_;
    my %Cfg = %{$Cfg};
    my $BaseOutName = $Cfg{'$OutFile'};

    $Cfg{'$ReadFile'} = 0;
    $Cfg{'$OutFile'} = "$BaseOutName.IsoE.test";
    $Cfg{'$OutputPar'} = " R P E T S ";

    open(OUT,">$BaseOutName.IsoE.cfg");$StreamOut = *OUT{IO};
    IsoEnergy(\%Cfg);
    close(OUT);
    system("urs_curve $BaseOutName.IsoE.cfg");

    $Cfg{'$TestClass'} = "TestClass UrsCurve_XY_Test { NameY1 Reader.2 NameY2 EOSIsoE.Pressure MaxMeanError 1e-006 }   }   ";
    $Cfg{'$TestVar'} = " TestClass ";
    $Cfg{'$ReadFile'} = 1;
    $Cfg{'$GetFromFile'} = $Cfg{'$OutFile'};
    $Cfg{'$GetFromFileColumn'} = 1;
    $Cfg{'$GetFromFile2Column'} = 3;
    $Cfg{'$OutFile'} = "$BaseOutName.IsoE.dat";
    $Cfg{'$GetFromFileNumColumn'} = 5;

    open(OUT,">$BaseOutName.IsoE.cfg");$StreamOut = *OUT{IO};
    IsoEnergy(\%Cfg);
    close(OUT);

}
sub GenerateTestHug{
    my ($Cfg) = @_;
    my %Cfg = %{$Cfg};
    my $BaseOutName = $Cfg{'$OutFile'};

    $Cfg{'$ReadFile'} = 0;
    $Cfg{'$OutFile'} = "$BaseOutName.Hug.test";
    $Cfg{'$OutputPar'} = " R P E U D T S ";
    $Cfg{'$SDenc'} = $Cfg{'$SHugDenc'};
    $Cfg{'$SEner'} = $Cfg{'$SHugEner'};

    open(OUT,">$BaseOutName.Hug.cfg");$StreamOut = *OUT{IO};
    Hugoniot_Pres(\%Cfg);
    close(OUT);
    system("urs_curve $BaseOutName.Hug.cfg");
    $Cfg{'$TestClass'} = "TestClass UrsCurve_XY_Test { NameY1 Reader.1 NameY2 Hug.Dencity MaxMeanError 1e-006 }   }   ";
    $Cfg{'$TestVar'} = " TestClass ";
    $Cfg{'$ReadFile'} = 1;
    $Cfg{'$GetFromFile'} = $Cfg{'$OutFile'};
    $Cfg{'$GetFromFileColumn'} = 2;
    $Cfg{'$OutFile'} = "$BaseOutName.Hug.dat";
    $Cfg{'$GetFromFileNumColumn'} = 7;

    open(OUT,">$BaseOutName.Hug.cfg");$StreamOut = *OUT{IO};
    Hugoniot_Pres(\%Cfg);
    close(OUT);

}
sub GenerateFreeTestConfig{
    my ($cfg_name) = @_;
    my %Cfg = ReadConf($cfg_name);
    GenerateTestHug(\%Cfg);
    GenerateTestIsoT(\%Cfg);

}
sub GenerateFree_ReducedTestConfig{
    my ($cfg_name) = @_;
    my %Cfg = ReadConf($cfg_name);
    GenerateTestIsoT_Reduced(\%Cfg);

}
sub GenerateCaloricTestConfig{
    my ($cfg_name) = @_;
    my %Cfg = ReadConf($cfg_name);
    GenerateTestIosE(\%Cfg);
    GenerateTestHug(\%Cfg);

}
use Getopt::Std;
use vars qw($Me);

sub usage {
    die <<EOF
usage: $Me [sr:cf:t:] [h] > out_file

Standard $Me options:
            $Me -s
   $Me -s conf_file_name
        s   generate config :
            input: <conf_file_name>
        t   generate test -IsoT Hug - config :
            input: <conf_file_name>
        e   generate test -IsoE Hug - config :
            input: <conf_file_name>
        r   generate test -IsoT - reduced output - R P T :
            input: <conf_file_name>
        .............
conf_file format:

\$Matter    MatterABu   Material_File material.cfg Substance Steel-Tern
\$HugRecalcHugP 1
\$HugStartPres  1e-4
// Isentrope is calculated faster but without U...
\$ClcIsentU     0
// Num fst pnt's
\$NumClcPnt 60
\$LogScale 0
// NumDifferent Energy(Temperature) points
\$NumClc2Pnt    1
\$Log2Scale     0
\$GetFromFileColumn 2

// Start point
\$SDenc 7.89
\$SPres 0
\$STemp 0
\$SEner 0

// Final point
\$FDenc 0
\$FPres 0
\$FTemp 0
\$FEner 0

// GetClcPoints
\$ReadFile 0
\$GetFromFile aaa.dat
\$GetFromFileColumn 1
\$GetFromFile2Column 2
\$GetFromFile3Column 3
\$GetFromFileNumColumn 7

\$CurveType Hugoniot_Denc  Hugoniot_Pres FreeE_IsoTherm IsoEntrope IsoEnergy FindTempE SplineFreeConstr
\$OutputPar R P E U D T S
\$OutFile  aaa.dat

// Additional params for full test
\$SHugDenc 0.07
\$SHugEner 0.1

// Params for FreeE spline generation
\$Spl_FileName spl_freeE.spl
\$Spl_Description FreeESpl
\$Spl_AddBeforeLogX  0
\$Spl_AddBeforeLogY  0
\$Spl_AddBeforeLogZ  205
\$Spl_GenerationMisfit 1e-6
\$Spl_GenerationNumX 200
\$Spl_GenerationNumY 200
\$Spl_MulX 1
\$Spl_MulY 1
\$Spl_MulZ 1

EOF
}

    ($Me = $0) =~ s!.*/!!;        # get basename of program,

    Getopt::Std::getopts("s:t:e:r:")       or usage();
    if ($Getopt::Std::opt_s){
        GenerateConfig($Getopt::Std::opt_s);
    } else{
        if ($Getopt::Std::opt_t){
            GenerateFreeTestConfig($Getopt::Std::opt_t);
        } else{
            if ($Getopt::Std::opt_e){
                GenerateCaloricTestConfig($Getopt::Std::opt_e);
            } else{
                if ($Getopt::Std::opt_r){
                    GenerateFree_ReducedTestConfig($Getopt::Std::opt_r);
                } else{
                         usage();
                }
            }
        }
    }
