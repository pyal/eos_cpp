############################################################################
#####  Spline Consistency check, draw, reconstruct #########################
############################################################################

use strict;
use warnings;
$/="\x0D\x0A";
$/="\x0A";
package Split3DFile;


############################################################################
sub new      #28.07.2005 15:35
############################################################################
 {
    my ($SrcName, $ResName, $NumColOut) = @_;
    my ($r) = {
        "SrcName"   =>$SrcName,
        "ResName"   =>$ResName,
        "NumRow"   =>0,
        "NumColOut"   =>$NumColOut
    };
    bless $r, "Split3DFile";
    $r->_ClcNumRow();
    return $r;
}
############################################################################
sub _ClcNumRow      #01.08.2005 12:32
############################################################################
 {
    my ($inp) = @_;
    open(IN, "< $inp->{SrcName}");
    $_=<IN>;
    my ($Num, $Prev) = (0,0);
    while ( <IN> ) {
        my @line = split " ", $_;
        $Prev = $line[0]            if ($Num==0);
        last                        if ( $line[0]!=$Prev );
        $Num++;
    }
    close(IN);
    $inp->{NumRow} = $Num;
}   ##_ClcNumRow
############################################################################
sub _Print      #28.07.2005 15:35
############################################################################
 {
 #no strict;
    my ($inp,$Hout,$FileName) = @_;
    open Out, "> ".$FileName;
    my ($i0,$i1,$i);
    for ( $i0=0;$i0<$inp->{NumRow};$i0++ ) {
        print Out "$Hout->{x}[$i0] ";
        for ( $i=0;$i<$inp->{NumColOut};$i++ ) {
            print Out "$Hout->{$i}[$i0] ";
        }
        print Out "\n";
    }
    close Out;
}   ##_Print
############################################################################
sub SplitFile       #28.07.2005 15:01
############################################################################
{
    my ($inp) = @_;
    my ($NumRow, $NumCol, $NumFiles) = (0, 0, 0);
    my %Hout = ();
    my $Hout = \%Hout;
    open( File, "< $inp->{SrcName} " );
    <File>;
    while (<File>) {
        chomp;
        my @line = split(" ", $_);
#print "OOH: ",join(" ",@line),"\n"  if ( int(@line)!=3 ) ;
        $Hout->{x}[$NumRow] = $line[1];
        $Hout->{$NumCol}[$NumRow] = $line[2];
        $NumRow++;
        if ($NumRow>=$inp->{NumRow}) {
            $NumRow = 0;
            $NumCol++;
            if ( $NumCol>= $inp->{NumColOut} ) {
                $NumFiles++;
                _Print($inp, \%Hout,$inp->{ResName}.sprintf("%03d",$NumFiles).".dat");
                $NumCol = 0;
                $NumRow = 0;
           }
        }
    }
    $NumFiles++;
    _Print($inp, \%Hout,$inp->{ResName}.sprintf("%03d",$NumFiles).".dat");
    close File;

}   ##SplitFile
############################################################################
sub _SetMean        #28.07.2005 16:56
############################################################################
 {
    my ($x0, $x1, $x2) = @_;

# 10 times larger diff -...... - better to calculate <dX^2>
    if ( abs($x0-$x1)*10<abs($x1-$x2) ) {
        return $x1;
    }
    if ( $x2<=$x0 ) {
        my $tmp = $x2;
        $x2 = $x0;
        $x0 = $tmp;
    }
#    if ( $x2<=$x0 ) {  return $x1;  }
    if ( $x1>$x2 || $x1<$x0  ) {
        $x1 = 0.5*($x0+$x2);
    }
    return $x1;
}   ##_SetMean
############################################################################
sub _CheckHash      #28.07.2005 16:51
############################################################################
 {
    my ($inp, $Hout, $out) = @_;
    my $k;
    for ( $k = 1;$k<$inp->{NumRow}-1;$k++ ) {
        $Hout->{$k}[2] = _SetMean($Hout->{$k-1}[2],$Hout->{$k}[2],$Hout->{$k+1}[2]);
    }
    for ( $k = 0;$k<$inp->{NumRow};$k++ ) {
        print $out "$Hout->{$k}[0] $Hout->{$k}[1] $Hout->{$k}[2]\n";
    }

}   ##_CheckHash
############################################################################
sub ConsistenceCheck        #28.07.2005 16:27
############################################################################
 {
#    no strict "refs";
    my ($inp, $MinNum) = @_;
    my ($NumRow, $NumCol, $NumFiles) = (0,0,0);
    my %Hout = ();
    my $Hout = \%Hout;
    open File, "< ".$inp->{SrcName};
    open Out, "> ".$inp->{ResName};
#    my $line = <File>;
    print Out (my $line=<File>);
#print " Min ",$MinNum," Out ", $inp->{ResName},"\x0D\x0A";
    while (<File>) {
        chomp;
        my @line = split " ", $_;
        $line[2] = $MinNum          if ($line[2]<$MinNum);
#print @line;
#print " ",$MinNum," ",@line[2]<$MinNum,"\x0D\x0A";
        @{$Hout->{$NumRow}} = @line;
        $NumRow++;
        if ($NumRow>=$inp->{NumRow}) {
            $NumRow = 0;
            $inp->_CheckHash(\%Hout,*Out{IO});
        }
    }
    close File;
    close Out;
}   ##ConsistenceCheck


#package main;


##my $splitP=Split3DFile::new("##tmpP","pres_view",150,10);
#my $splitT=Split3DFile::new("##tmpT","temp_view",150,10);
##$split->SplitFile;
#$splitT->ConsistenceCheck(1e-6);
#$splitT->SplitFile;






package main;

use Getopt::Std;
use vars qw($Me);

@main::Input = ("##tmpP", "##tmpT");
@main::Output = ("tmp_pres.dat", "tmp_temp.dat");
@main::View = ("view_pres", "view_temp");

sub usage {
    die <<EOF

usage: $Me [t:n:m:o:vc]  [h] < in_file

reads  $main::Input[0], $main::Input[1]

writes $main::Output[0] $main::Output[1]

View   $main::View[0] , $main::View[1]  - default valude

Input - in the format
        TColumn  NumberCol 3 NumberRow  22500
                    0.5         0.0001       0.762536
                    .....................


Standard $Me options:
        t 0    type of data to work with 0 = Pressure 1 = Temperature
        o out  Set output file name
        n 10   Is setted for -v option - determines how many columns will be in output file
        m 1e-6 Is setted for -c option - determines minimum number in the input data

            $Me -v -o out_file -i in_file -t 0 -n 10  <in_file
        v   split an input file (t = 0 -> $main::Input[0] t = 1 -> $main::Input[1] )
                                 and generate a set of output files out_file_001.dat .... out_file_00N.dat -
                                 with  NumColumns is setted by -n option
                                 default out_file (t = 0 -> $main::View[0] t = 1 -> $main::View[1] )
                                 default in_file (t = 0 -> $main::Input[0] t = 1 -> $main::Input[1] )

            $Me -c -o out_file -i in_file -t 0 -m 1e-6
        c   consistency checks input file (t = 0 -> $main::Input[0] t = 1 -> $main::Input[1] )
                                 will try to remove spurious oscillations in the input data
                                 to set data more then minimum value (-m option)
                                 default out_file (t = 0 -> $main::Output[0] t = 1 -> $main::Output[1] )
                                 default in_file (t = 0 -> $main::Input[0] t = 1 -> $main::Input[1] )
        .............

EOF
}

    ($Me = $0) =~ s!.*/!!;        # get basename of program,
    Getopt::Std::getopts("t:n:m:o:i:v:c")          or usage();

    my $kind = $Getopt::Std::opt_t;
    die " have to set -tx parameter 0<x<1 \n"    if ($kind<0 or $kind>1);
#    my $Length = $Getopt::Std::opt_l;
#    die " have to set -lx parameter 0<x \x0D\x0A"      if ($Length<=0);

    if ($Getopt::Std::opt_v){
        my $NumCol = $Getopt::Std::opt_n;
        $NumCol = "15"    if (!defined($NumCol));
        die " have to set -nx parameter 0<x Now NumCol $NumCol\n"  if ($NumCol<=0);
        my $In = $main::Input[$kind];
        $In = $Getopt::Std::opt_i               if ( $Getopt::Std::opt_i ) ;
        my $Out = $main::View[$kind];
        $Out = $Getopt::Std::opt_o               if ( $Getopt::Std::opt_o ) ;
#print $Getopt::Std::opt_v, " ", $Getopt::Std::opt_v eq "", " ",$Out,"\x0D\x0A";
        my $Split=Split3DFile::new($In,$Out,$NumCol);
        $Split->SplitFile;
    }
    else{
        if ($Getopt::Std::opt_c){
            my $In = $main::Input[$kind];
            $In = $Getopt::Std::opt_i               if ( $Getopt::Std::opt_i ) ;
            my $Out = $main::Output[$kind];
            $Out = $Getopt::Std::opt_o               if ( $Getopt::Std::opt_o ) ;
            die "have to set -m value"               if ( !$Getopt::Std::opt_m ) ;
            my $MinVal = $Getopt::Std::opt_m;
            my $Split=Split3DFile::new($In,$Out,1);
            $Split->ConsistenceCheck($MinVal);
        }
        else{
                usage();
            }
        }
