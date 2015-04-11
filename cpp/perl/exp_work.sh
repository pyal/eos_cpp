#!/usr/local/bin/bash


ShrinkSum(){
    for ii in $(ls *.$1); do
        cat $ii | awk 'BEGIN{RS="\x0D\x0A"}{if ( $1+0>'$2'+0 && $1+0<'$3'+0 ) print $0;}' >$ii.res
    done
    set1grph $(ls *.res) !sum.dat /i

}

CreateConfig(){







    perl5.6.1 -e '{
	    use strict;
	    use warnings;
	    $/="\x0D\x0A";
#print "Test $/	    ";
	    my ($Config) = $ARGV[0];
	    open(IN,"< $Config")		or die "Cannot open config file $Config\n";
	    my %convert;
	    my $line;
	    while (1){
	        $line = <IN>;
	        chomp($line)        if (length($line)>2);
print $line,"\n";
	        last                    if (substr($line,1,2) eq "==");
	        my @line = split(" ",$line);
	        next                    if (int(@line)!=2);
	        $convert{$line[0]} = $line[1];
	    }
	    die "In config have to define vars: InFile_ FrCol_ \n"  
	        if ( !defined($convert{InFile_}) || !defined($convert{FrCol_}) );
	    my @name = split(/\./,$convert{InFile_});
print "name was $convert{InFile_} split ",join(":",@name),"\n";	    
	    pop(@name)                 if (int(@name)>1);
	    my $name = join("\.",@name)."_".$convert{FrCol_}."\.fm";
print "name was $convert{InFile_} split ",join(":",@name)," res $name\n";	    
	    open(OUT,"> $name")     or die "Could not open file $name for writing\n";
	    print OUT $line,"\n";
	    while (<IN>){
	        chomp           if (length($_)>2);
	        $line = $_;
	        while( my ($key,$val) = each(%convert) ){
	            $line =~ s/$key/$val/gi;
	        }
	        print OUT $line,"\n";
	    }
	    close(OUT);close(IN);
    }' $1
}







Usage(){
    echo 'usage: '$0' -s DAT 11000 14000'
    echo '              all .dat files in the directory will be shrink 11000<x<14000'
    echo '              there addded extention will be .res '
    echo '              Extention (dat) - is case sensitive (DAT -is different) '
    echo '              after that they will be put into !sum.dat file'
    echo '       '$0' -c appr_pp.fm'
    echo '              Will read appr_pp.fm and copy it to InFile_FrCol.fm'

}


while getopts "sc" opt; do
    case $opt in
    s  ) SHRINK_SUM=1;;
    c  ) CREATE_CONFIG=1;;
    \? )  Usage
         exit 1
    esac
done

if   [ ! -z $SHRINK_SUM ]
then
    ShrinkSum $2 $3 $4 $5
    exit 1
fi
if   [ ! -z $CREATE_CONFIG ]
then
    CreateConfig $2
    exit 1
fi

Usage