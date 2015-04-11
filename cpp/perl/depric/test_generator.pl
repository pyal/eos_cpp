#!/usr/local/bin/perl5.6.1 
use strict;
use warnings;
no strict "refs";
$/="\x0D\x0A";

my $StdConfigFile = "test_all.pl.cfg";
my $StdConfigGenerator = "test.all.pl";
my $TmpStdConfig = "##tmp.cfg";
my $StdCurveGenerator = "test_urs.exe";
my $SectionName = '#EndData';

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
# Read StdConfig = generate TmpStdConfig file
# with added parameters from %SubstVars
sub GenerateTmpConfig{
    my ($SubstVars) = @_;
    open(IN,"< $StdConfigFile")     or die "Could not read StdConf file $StdConfigFile Error $!\n";
    open(OUT,"> $TmpStdConfig")     or die "Could not write to TmpStdConf file $TmpStdConfig Error $!\n";
    while (<IN>){
        chomp;
        my @l = split(" ",$_);
        next                if (int(@l)==0 || substr($l[0],0,2) eq "//");
        last                if ($l[0] eq $SectionName);
        print OUT "$_\n";
    }
    while ( my ($key, $val) = each(%{$SubstVars}) ){
        print OUT "$key $val\n"     if (defined($key) && defined($val));
    }
    print OUT "$SectionName\n";
    while (<IN>){
        chomp;
        my @l = split(" ",$_);
        print OUT "$_\n";
    }
    close(IN);
    close(OUT);
}
# Generate from initial %Conf -> %SubstVars to be used in GenerateTmpConfig
sub GenerateIsoEvar{
    my ($Conf,$OutDat) = @_;
    my %Conf = %{$Conf};
    my %AddVar=(
        '$FromD',$Conf{'$MinDenc'},     '$ToD',$Conf{'$MaxDenc'},         '$LogScaleD',1, '$NumSameD',1 , 
        '$FromE',$Conf{'$MinE'},        '$ToE',$Conf{'$MaxE'},            '$LogScaleE',1, '$NumSameE',$Conf{'$NumDencPnt'} ,
        '$NumPntD',$Conf{'$NumDencPnt'}, '$NumPntE',$Conf{'$NumSameDencPnt'}, '$NumPnt', $Conf{'$NumDencPnt'}*$Conf{'$NumSameDencPnt'},
        '$OutType','IsoEOutSimple',
        '$PerlLeaveVar','$$OutType Matter',
        '$FileOutPar', '$$OutType',
        '$Matter', $Conf{'$Matter'}

    );
    if (!$OutDat){
        $AddVar{'$PerlLeaveVar'} = '$$OutType Matter TestClass1 Reader';
        $AddVar{'$FileOutPar'} = '$$OutType TestClass1';
    }
    return %AddVar;
}
sub GenerateHugVar{
    my ($Conf,$OutDat) = @_;
    my %Conf = %{$Conf};
    my %AddVar=(
        '$FromP',$Conf{'$HugP'},        '$ToP',$Conf{'$MaxP'},            '$LogScaleP',1, '$NumSameP',1 ,
        '$NumPnt', $Conf{'$NumDencPnt'}, '$NumPntP', $Conf{'$NumDencPnt'},
        '$OutType','HugOutSimple',
        '$PerlLeaveVar','$$OutType Matter $NameDencHug',
        '$NameDencHug', 'Pressure',
        '$FileOutPar', '$$OutType',
        '$Matter', $Conf{'$Matter'},

        '$StartDHug', $Conf{'$HugDenc'},
        '$StartEHug', $Conf{'$HugE'},
        '$StartPHug', $Conf{'$HugP'},
        '$StartUHug', 0,
        '$PresDepHug', 1,
        '$StartClcPHug', $Conf{'$HugRecalcP'}

    );
    if (!$OutDat){
        $AddVar{'$PerlLeaveVar'} = '$$OutType Matter $NameDencHug TestClass0 Reader';
        $AddVar{'$FileOutPar'} = '$$OutType TestClass0';
    }
    return %AddVar;
}
sub GenerateIsentropeVar{
    my ($Conf,$MinIsent,$OutDat) = @_;
    my %Conf = %{$Conf};
    my %AddVar=(
        '$StartEIsent', $Conf{'$HugE'},
        '$FromD',$Conf{'$HugDenc'},     '$ToD',$Conf{'$MaxDenc'},         '$LogScaleD',0, '$NumSameD',1 , 
        '$NumPnt', $Conf{'$NumDencPnt'}, '$NumPntD', $Conf{'$NumDencPnt'},
        '$OutType','IseOutSimple',
        '$PerlLeaveVar','$$OutType Matter ',
        '$FileToStore', '$Matter[0].$Matter[-1].$OutType'.$MinIsent,
        '$FileOutPar', '$$OutType',
        '$Matter', $Conf{'$Matter'},

        '$StartUIsent', '0.'

    );
    if (!$OutDat){
        $AddVar{'$PerlLeaveVar'} = '$$OutType Matter $NameDencHug TestClass1 Reader';
        $AddVar{'$FileOutPar'} = '$$OutType TestClass1';
    }
    if ($MinIsent eq "Max"){
        $AddVar{'$StartEIsent'} = $Conf{'$MaxE'};
        $AddVar{'$FromD'} = $Conf{'$MaxDenc'};  
        $AddVar{'$ToD'} = $Conf{'$HugDenc'};
    }
    return %AddVar;
}
sub GetOutName{
    my ($name) = @_;
    open(IN,"< $name")      or die "Could not open cfg file (to get OutName) $name; Error $!\n";
    while (<IN>){
        chomp;
        my @l = split(" ",$_);
        for(my $i=0;$i<int(@l);$i++){
            next            if ( !($l[$i] eq "FileToStore") );
            return $l[$i+1] if ($i+1<int(@l));
            while (<IN>){
                chomp;
                my @l = split(" ",$_);
                next        if (int(@l)==0);
                return $l[0];
            }
        }
    }
    die "Could not find in file $name the string FileToStore \n";
}
sub MakeTestCurve{
    my @par = @_;
    my $ToDo = shift(@par);
    my %AddVar=&$ToDo(@par,1);
    GenerateTmpConfig(\%AddVar);
    system("perl5.6.1 $StdConfigGenerator $TmpStdConfig $TmpStdConfig.res");
    system("$StdCurveGenerator $TmpStdConfig.res");
    my $OutName = GetOutName("$TmpStdConfig.res");
    $OutName = substr($OutName,0,length($OutName)-4);
    
    %AddVar=&$ToDo(@par,0);
    GenerateTmpConfig(\%AddVar);
    system("mv $OutName.dat $OutName.test");
    system("perl5.6.1 $StdConfigGenerator $TmpStdConfig $OutName.cfg");
    system("rm $TmpStdConfig $TmpStdConfig.res");
}
sub GenerateCaloricTest{
    my ($ConfName) = @_;
    my %Conf = ReadConf($ConfName);
    MakeTestCurve("GenerateIsoEvar",\%Conf);
    MakeTestCurve("GenerateHugVar",\%Conf);
    MakeTestCurve("GenerateIsentropeVar",\%Conf,"Min");
    MakeTestCurve("GenerateIsentropeVar",\%Conf,"Max");
    
}
sub GenerateFreeETest{
    my ($ConfName) = @_;
    my %Conf = ReadConf($ConfName);
    MakeTestCurve("GenerateIsoEvar",\%Conf);
}
use Getopt::Std;
use vars qw($Me);

sub usage {
    die <<EOF
usage: $Me [sr:cf:t:] [h] > out_file

Standard $Me options:
            $Me -s 
   $Me -s conf_file_name
        s   make caloric test files : 
            IsoE table (MinD MaxD MinE MaxE)
            Hug (HugD)
            IsentLow ( MinE - (HugD-MaxD) )
            IsentUp  ( MaxE - (MaxD-HugD) )
        .............
conf_file format:       
\$MinDenc 1e-5
\$MaxDenc 2
\$HugDenc 1e-1
\$HugP 1e-4
\$HugE 0.1
\$HugRecalcP 1
\$MaxP 1e6
\$MinE 0.1
\$MaxE 1e6
\$NumDencPnt 60
\$NumSameDencPnt 60
\$Matter	MatterABu   Material_File material.cfg Substance Steel-Tern


EOF
}

    ($Me = $0) =~ s!.*/!!;        # get basename of program,

    Getopt::Std::getopts("s:")       or usage();
    if ($Getopt::Std::opt_s){
        GenerateCaloricTest($Getopt::Std::opt_s);
    }
    else{
                            usage();
    }



