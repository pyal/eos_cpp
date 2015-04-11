use strict;
use warnings;

my %Extract_rules = (" P  [bar]  "=> 1,
#                    " T  [K]    "=>-1,
                     " Ro [g/cc] "=> 0,
#              " Ne/(Na+Ni)", 3,
                     " e   "=> 2,
                     " Ne  "      => 3,
                     " Ne+ "      => 4,
                     " Ne+2"      => 5
);
my $Delimiter = "                              Thermodynamic functions";

open(IN,"<$ARGV[0] ")           or die "Could not open file $ARGV[0]: \n$!\n";
############################################################################
sub TestLine        #09/05/2007 1:40
############################################################################
 {
    my ($test_line, $input_line, $res) = @_;
    return 0        if ($test_line ne substr($input_line,0,length($test_line)));
    my $l = substr($input_line,length($test_line));
    my @tmp = split(" ",$l);
#print(" Input\n<$input_line> Test : <$test_line> ResL <$l>"), return 1             if (int(@tmp)==0);
    ${$res} = $tmp[0];
#print " Input\n$input_line Test : $test_line ResL $l Res $tmp[0] ${$res}\n";
    return 1;
}   ##TestLine
############################################################################
sub ReadIter        #09/05/2007 1:25
############################################################################
 {
    my ($dat_res, $head, $rules) = @_;
    @{$dat_res} = ();
    while ( <IN> ) {
        my $line = $_;
        my $res;
        return 1                if (TestLine($head, $line, \$res));
        while( my ($key, $val) = each(%{$rules}) ){
            $dat_res->[$val] = $res     if (TestLine($key, $line, \$res));
        }
    }
    return 0;
}   ##ReadIter

my @dat;
while( my ($key, $val) = each(%Extract_rules) ){
    $key =~  s/ //g;
    $dat[$val] = $key;
}
print(join("\t",@dat),"\n");
while(ReadIter(\@dat, $Delimiter, \%Extract_rules)){
    next    if (int(@dat)==0);
    for(my $i=0;$i<int(@dat);$i++){
        printf("$dat[$i]\t");
    }
    printf("\n");
}



#cat Ne-RoT2.dat | gawk 'NR^>1{n=$3+$4;print $1,$2*1e-4,$4/n,$5/n,$6/n}' >Ne-RoT2.dat.dat