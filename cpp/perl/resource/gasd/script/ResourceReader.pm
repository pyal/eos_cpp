#!/usr/local/bin/perl5.6.1

package ResourceReader;
use strict;
use warnings;
############################################################################
sub new {
    my ($res_name) = @_;
    my ($r) = {
        ResName =>$res_name
    };
    bless $r, "ResourceReader";
    $r->FindInc();
    return $r;
}   ##new

############################################################################
sub FindKey{ # Static 
	my ($File, $Key) = @_;
	while(<$File>){
		my @line = split(" ",$_);
		my %tmp;
		@tmp{@line} = ();
		return @line		if (exists($tmp{$Key}));
	}
	return;
}

############################################################################
sub FindInc{
	my ($ptr) = @_;
	my $ResFile = $ptr->{ResName};
	open(INC,"< $ResFile")              or die "FindInc:Could not open file $ResFile\n";
	my $IncName;
	while(1){
	    my @line = FindKey(*INC{IO},"#include");
	    die	"Cannot find include file in resource file $ResFile\n"	if (int(@line)<2);
	    $IncName = $line[1];
	    last            if (! ($IncName eq "afxres.h") );
	}
	close(INC);
	$_ = $IncName;
	$IncName =~ s/\"//g; 
	$ptr->{IncName} = $IncName;
}

############################################################################
sub MakeConstName{ # Static
	my ($ToAddFile) = @_;
	my $name = $ToAddFile;
	$name =~ tr/\/\\./||_/;
	my @name = split(/\|/,$name);
	return uc(pop(@name));
}

############################################################################
sub FindBefore{
	my ($IncFile,$Num) = @_;
	open(IN,"< $IncFile")                   or die "FindBefore:Could not open file $IncFile\n";
	my ($Before,$Max, $LastVal);
	while (1){
	    my @line = FindKey(*IN{IO},"#define");
#	    if (int(@line)<3 || int($line[2])==0 || (substr($line[1],0,10) eq "_APS_NEXT_") ){
	    if ( substr($line[1],0,10) eq "_APS_NEXT_" ){
#we do not have any numbers defined...
	        $LastVal = $line[2]        if ( $line[1] eq "_APS_NEXT_RESOURCE_VALUE" );
	        last;
        }
	$line[2] = -2		if (!defined($line[2]));
	    $Max = int($line[2]);
        last            if ($Max>$Num && $Num!=-1 );
        $Before = $line[1];
    }
    if ( !defined($LastVal) ){
        my @line = FindKey(*IN{IO},"_APS_NEXT_RESOURCE_VALUE");
        die " Could not find :_APS_NEXT_RESOURCE_VALUE: - bad include format and Num==-1\n"    if ( int(@line)<3 );
        $LastVal = $line[2];
    }
    if ($Num==-1){
        if (defined($Max)){
            $Num = $Max+1>$LastVal ? $Max+1 : $LastVal;
        } else {
            $Num = $LastVal;
        }
    }
    my $MaxNum = $Num+1>$LastVal ? $Num+1 : $LastVal;
    close(IN);
    $Before = ""                if (!defined($Before));
    return ($Before,$Num,$MaxNum);
}

############################################################################
sub AddInc{
	my ($ptr,$Num_,$ToAddFile) = @_;
	my $IncFile = $ptr->{IncName};
	my ($ConstBefore, $ConstNum, $MaxNum) = FindBefore($IncFile, $Num_);
#print "ConstBefore $ConstBefore ConstNum $ConstNum  $MaxNum MaxNum\n";	
	system("cp $IncFile $IncFile.bak");
	open(INCL,"< $IncFile.bak")         or die "AddInc:Could not open file $IncFile.bak\n";
	open(OUT,"> $IncFile")              or die "FindInc:Could not open out file $IncFile\n";
	while(1){
	    my $line = <INCL>;
	    if (!defined($line)){
	        close(OUT);close(INCL);
            system("cp $IncFile.bak $IncFile");
	        die " Could not understand format of the file $IncFile\n";
        }
        print OUT chopm($line),"\n";
	    last            if ( ($ConstBefore eq "") && (substr($line,0,2) eq "//") );
	    my @line = split(" ", $line);
	    last            if ( int(@line)>2 && !($ConstBefore eq "") && ($line[1] eq $ConstBefore) );
	}
	my $ConstName = MakeConstName($ToAddFile);
	print OUT "#define $ConstName $ConstNum\n";
	while(<INCL>){ 
	    my @line = split(" ",$_);
	    if (int(@line)==3 && ($line[1] eq "_APS_NEXT_RESOURCE_VALUE")){
	        print OUT $line[0]," ",$line[1]," \t ",$MaxNum,"\n";
	    } else { 
	        print OUT chopm($_),"\n";
	    }
	}
    close(INCL);close(OUT);
    return ($ConstBefore, $ConstNum, $ConstName);
}

############################################################################
sub DelInc{
	my ($ptr,$ToDelFile) = @_;
	my $IncFile = $ptr->{IncName};
	system("cp $IncFile $IncFile.bak");
	open(INCL,"< $IncFile.bak")         or die "AddInc:Could not open file $IncFile.bak\n";
	open(OUT,"> $IncFile")              or die "FindInc:Could not open out file $IncFile\n";
	my $ConstName = MakeConstName($ToDelFile);
	while(<INCL>){
        my @line = split(" ",$_);
	    next     if ( int(@line)>2 && ($line[0] eq "#define") && ($line[1] eq $ConstName) );
        print OUT chopm($_),"\n";
	}
    close(INCL);close(OUT);
    return ($ConstName);
}

sub ModifName{
    my ($Name) = @_;
#    $_ = $Name;
#	$Name =~ tr/\//;
    return $Name;
}

############################################################################
sub AddRes{
	my ($ptr,$ToAddFile,$ConstBefore, $ConstNum, $ConstName) = @_;
	$ToAddFile = ModifName($ToAddFile);
	my $ResourceFile = $ptr->{ResName};
	system("cp $ResourceFile $ResourceFile.bak");
	open(INRES,"< $ResourceFile.bak")       or die "AddRes:Could not open file $ResourceFile.bak\n";
	open(OUT,"> $ResourceFile")             or die "AddRes:Could not open out file $ResourceFile\n";
	while(1){
	    my $line = <INRES>;
	    if (!defined($line)){
	        close(OUT);close(INRES);
            system("cp $ResourceFile $ResourceFile.tmp");
            system("cp $ResourceFile.bak $ResourceFile");
	        die " Could not understand format of the file $ResourceFile\n";
        }	                
        print OUT chopm($line), "\n";
        if ( ($ConstBefore eq "") && (substr($line,0,6) eq "// TXT") ){
            $line = <INRES>;
            print OUT chopm($line),"\n";
            if (substr($line,0,2) eq "//"){
                $line = <INRES>;
                print OUT chopm($line),"\n";
                last;
            } 
        }
	    my @line = split(" ", $line);
	    last            if ( !($ConstBefore eq "") && defined($line[0]) && ($line[0] eq $ConstBefore) );
	}
	print OUT "$ConstName \t TXT \"$ToAddFile\"\n";
	while(<INRES>){ print OUT chopm($_),"\n";}
    close(INRES);close(OUT);
}
############################################################################
sub DelRes{
	my ($ptr,$ConstName) = @_;
	my $ResFile = $ptr->{ResName};
	system("cp $ResFile $ResFile.bak");
	open(INCL,"< $ResFile.bak")         or die "DelRes:Could not open file $ResFile.bak\n";
	open(OUT,"> $ResFile")              or die "DelRes:Could not open out file $ResFile\n";
	while(<INCL>){
        my @line = split(" ",$_);
	    next     if ( int(@line)>2 && ($line[1] eq "TXT") && ($line[0] eq $ConstName) );
        print OUT chopm($_),"\n";
	}
    close(INCL);close(OUT);
    return ($ConstName);
}

############################################################################
#####                       For External use                          ######
############################################################################
sub AddResource{ #$NumToAdd =-1 - to add new number
    my ($ptr, $ToAddFile, $NumToAdd ) = @_;
    my ($ConstBefore, $ConstNum, $ConstName) = $ptr->AddInc($NumToAdd,$ToAddFile);
	$ptr->AddRes( $ToAddFile, $ConstBefore, $ConstNum, $ConstName);
	$ptr->{ToAddFile} = $ToAddFile;
	$ptr->{ConstName} = $ConstName;
	$ptr->{ConstNum} = $ConstNum;
}
sub DelResource{
    my ($ptr, $ToDelFile) = @_;
    my ($ConstName) = $ptr->DelInc($ToDelFile);
	$ptr->DelRes($ConstName);
}
sub PrintHash{
    my ($h) = @_;
    while( my ($key, $val) = each(%{$h}) ){
        print $key,"\t",$val,"\n";
    }
}
sub chopm{
    my ($x) = @_;
    $_ = $x;
    $x =~ s/\x0D|\x0A//g;
    return $x;
}
1;
 