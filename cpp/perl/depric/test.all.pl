#!/usr/local/bin/perl5.6.1 
use strict;
use warnings;
no strict "refs";
$/="\x0D\x0A";



sub GetSubstVar{
    my ($line,$SubstVars) = @_;
    my @l = split " ",$line;
    my (@tmp, $tmp);
    if (int(@l)==1){
        @tmp = split(" ",<IN>);
        $SubstVars->{$l[0]} = \@tmp;
    } else {
        @tmp = @l;
        $tmp = shift(@tmp);
        $SubstVars->{$tmp} = \@tmp;
    }

}
sub MakeSubstitution{
    my ($line, $subst) = @_;
    my $res = 1;
    while ($res) {
        $res = 0;
        foreach my $key ( sort {$b cmp $a} keys(%{$subst}) ){
            my $val = $subst->{$key};
            my $subs = join(" ",@{$val});
	        $key =~ s/\$/\\\$/gi;
            my $s = $line;
            if ($s =~ /$key\[([+-]?\d+)\]/) {
                $res = 1;
                my $i = $1;
                $i = int(@{$val})-1           if ($i<0);
                $line =~ s/$key\[$1\]/$val->[$i]/gi
            } else {
	            $res = $res || ($line =~ s/$key/$subs/gi);
	        }
	    }
	}
    return $line;
}
sub GetLeaveVar{
    my ($PerlLeaveVar,$SubstVars) = @_;
    my %ListClc;
    return %ListClc             if (!defined($PerlLeaveVar));
    my @lst = @{$PerlLeaveVar};
    @lst = split(" ",MakeSubstitution(join(" ",@lst), $SubstVars));
    while (my $Par=shift(@lst) ){
        my @tmp = split(/\./,$Par);
        $ListClc{$tmp[0]} = 1;
    }
    return %ListClc;
}
sub ReadUpToSection{
    my @pars = @_;
    my $SectionName = shift(@pars);
    my $ToDo = shift(@pars);
    my @l = ("");
    while(<IN>){
        chomp;#        if (length($_)>2);
        my @l = split(" ",$_);
        next                if (int(@l)==0 || substr($l[0],0,2) eq "//");
        last                if ($l[0] eq $SectionName);
        &{$ToDo}($_,@pars)     if (defined($ToDo));
    }
}
sub DirectPrint{
    my ($line,$SubstVars,$List) = @_;
    $line = MakeSubstitution($line, $SubstVars);
    if (!defined($List)){
        print OUT $line,"\n";
    } else {
        my @l = split(" ",$line);
        print OUT $line,"\n"             if (defined($List->{$l[0]}));
    }
}

my %SubstVars;
open(IN,"< $ARGV[0]")       or die "Could not read config file $ARGV[0] error $!\n";
open(OUT,"> $ARGV[1]")      or die "Could not open result file $ARGV[1] error \:$!\n";
ReadUpToSection("#EndData","GetSubstVar",\%SubstVars);
my %SubstNew = %SubstVars;
while( my ($key,$val) = each(%SubstVars) ){
    my @tmp = split(" ",MakeSubstitution(join(" ",@{$val}), \%SubstNew));
    $SubstVars{$key} = \@tmp;
}
my %LeaveVar = GetLeaveVar($SubstVars{'$PerlLeaveVar'},\%SubstVars);
ReadUpToSection("#EndHead","DirectPrint",\%SubstVars);
ReadUpToSection("#EndVariables","DirectPrint",\%SubstVars,\%LeaveVar);
ReadUpToSection("#End","DirectPrint",\%SubstVars);




