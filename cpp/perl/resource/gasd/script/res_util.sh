#!/usr/local/bin/bash


UpdateResource() {
    CURDIR=$(pwd)
    ResDir=$(pwd)
    ResourceDir=$1
    cd $ResourceDir
    TODIR=$(pwd)
    cd $CURDIR
    cd $ResDir
    for ii in $(ls *.res); do
      cat $ii |
	awk ' BEGIN{s="'$TODIR'/'$ii'";s=substr(s,1,length(s)-4)".dat";print s}
	     {
	      print $1,"\t",$3 >s
	     }'
    done
}
ConfigDo(){
#    perl5.6.1 -e '{
    perl -e '{
	use strict;
	use warnings;
	my @tmp = split(/\./,$ARGV[0]);
	my $src = "###tmp";
	system("cp ".$ARGV[0]." ".$src);
	system("famessb $src /o /v");
	pop(@tmp);
	my $name = join("\.",@tmp);
	my ($cfg,$dat,$res) = ($name.".cfg", $name.".dat", $name.".res");
	open(IN,"< $src")		or die "Could not open $src\n";
	open(CFG,"> $cfg")		or die "Could not write $cfg\n";
	my ($NotDone, $ResFile) = (1,"");
	while(<IN>){
	    $_ =~ s/\x0D|\x0A//g;
	    my @line = split(" ",$_);
	    if ($NotDone && ($line[0] eq "GeneralBegin") ){
		print CFG "GeneralBegin  $dat  $res\n";
		$ResFile = $line[2];
		$NotDone = 0;
	    } else {
	    	print CFG $_,"\n";
	    }
	}
	open(DATFROM,"< $ResFile")	or die "Could not open $ResFile\n";
	open(DATTO,"> $dat")		or die "Could not write $dat\n";
	while(<DATFROM>){
	    my @line = split(" ",$_);
	    my @last = @line;
	    shift(@last);shift(@last);
	    print DATTO $line[0], " ",join(" ",@last),"\n";
	}

    }' $1
}
AddResource(){
#    perl5.6.1 -e '{
    perl -e '{
	    use strict;
	    use warnings;
	    my ($ResourceFile,$ToAddFile,$NumToAdd) = ($ARGV[0],$ARGV[1],$ARGV[2]);
	    require ResourceReader;
	    $_ = $ToAddFile;
	    tr/\/\\/||/;
      s/\|\|/\|/g;
      s/\.\|//g;
	    s/\|/\\\\/g;
	    $ToAddFile = ".\\\\".$_;
	#print $ToAddFile,"\n";exit 0;
	    $NumToAdd = -1	if (!defined($NumToAdd));

	    my $read = ResourceReader::new($ResourceFile);
	    $read->AddResource($ToAddFile,$NumToAdd);
    }' $1 $2 $3
}
DelResourceByFile(){
#    perl5.6.1 -e '{
    perl -e '{
	    use strict;
	    use warnings;
	    my ($ResourceFile,$ToDelFile) = ($ARGV[0],$ARGV[1]);
	    require ResourceReader;
	    my $read = ResourceReader::new($ResourceFile);
	    $read->DelResource($ToDelFile);
    }' $1 $2
}

Usage(){
    echo 'usage:'$0' '
    echo '        -u ResourceDir              '
    echo '	         update resource by new .res files '
    echo '        -c famessb.cf                      '
    echo '	         prepare config file:'
    echo '	         set good in out files, + create test(.res)'
    echo '        -a resource.rc famessb.cf -1           '
    echo '	         add file to the resource *.rc:'
    echo '	              make uppercase constant with number (-1 - max number)'
    echo '        -d resource.rc famessb.cf            '
    echo '	         del file from the resource *.rc *.h:'
}


while getopts "ucad" opt; do
    case $opt in
    u  ) Update=1;;
    c  ) Config=1;;
    a  ) ResAdd=1;;
    d  ) ResDel=1;;

#     w  ) DistantQuery=$OPTARG;;
    \? )  Usage
         exit 1
    esac
done
if   [ ! -z $Update ]
then
    UpdateResource $2
    exit 1
fi
if   [ ! -z $Config ]
then
    ConfigDo $2
    exit 1
fi
if   [ ! -z $ResAdd ]
then
    AddResource $2 $3 $4
    exit 1
fi
if   [ ! -z $ResDel ]
then
    DelResourceByFile $2 $3
    exit 1
fi

Usage


























#
# GenerateOutputCodes() {
# #    GenerateOutputCodes "$Query" "$DBFile" "$RESDIR" "$REMOTE_HOST"
#
# Query=$1
# DBFile=$2
# RESDIR=$3
# REMOTE_HOST=$4
# #echo REMOTE_HOST:$REMOTE_HOST:
# if [ "$REMOTE_HOST" = "" ]; then
# REMOTEDO=
# REMOTECOPY='cp '
# else
# REMOTEDO='rsh '$REMOTE_HOST
# REMOTECOPY='rcp '$REMOTE_HOST:
# fi
# #echo Query:$Query:DBFile:$DBFile:RESDIR:$RESDIR:REMOTE_HOST:$REMOTE_HOST:
# #echo REMOTEDO:$REMOTEDO:REMOTECOPY:$REMOTECOPY:
# #echo        cat $Query   '|' $REMOTEDO $RESDIR/porno2 -y $DBFile -q /dev/stdin -o /dev/stdout -m '|' awk -F ''\t'' ''{ print $0 }'' '>'$Query.res
# #        cat $Query   | $REMOTEDO $RESDIR/porno2 -y $DBFile -q /dev/stdin -o /dev/stdout -m | awk -F '\t' '{ print $0 }'|  >$Query.res
#         cat $Query   | $REMOTEDO $RESDIR/porno2 -y $DBFile -q /dev/stdin -o $RESDIR/$Query.tmp -m
#         cat $Query.tmp | awk -F '\t' '{ print $0 }'  >$Query.res
#         $REMOTECOPY$DBFile"arr" indexarr.tmp
# INDEXFILE=indexarr.tmp
#
#
# #echo done OK!!! perl:
#
#
#     perl -e '{
#         package CVT_IND;
#         use strict;
#         ############################################################################
#         #01.09.2005 13:04       new
#         sub new {
#             my ($cvt_name) = @_;
#             my ($r) = {};
#             bless $r, "CVT_IND";
#             $r->ReadFilter_($cvt_name);
#             return $r;
#         }   ##new
#         sub ReadFilter_{
#             my ($this, $cvt_name) = @_;
#             no strict "subs";
#             if (not open(F, "< $cvt_name")){
#                 print STDERR "file not found ",$cvt_name,"\n";
#                 return;
#             }
#             binmode(F);
#             my $buf;
#             my $num = 0;
#             my @cvt;
#             while(sysread(F, $buf, 4)){
# #print $num," ",  unpack ("I1", $buf),"\n"               if ($num%10000==0);
#                 @cvt[$num] = unpack ("I1", $buf);
#                 $num++;
#             }
#             close(F);
#             $this->{Filter}=\@cvt;
#         }
#         sub Do{
#             my ($this, $in_name, $out_name) = @_;
#             if ( not defined($this->{Filter}) ) {
#                 system("mv ".$in_name." ".$out_name);
#                 return;
#             }
#             open(in_tmp,"< ".$in_name);
#             open(out_tmp,"> ".$out_name);
#             while (<in_tmp>){
#                 my @line = split " ",$_;
#                 for (my $k = 0;$k<int(@line);$k++){
# #print @line[$k]," ", $this->{Filter}[int(@line[$k])],"\n";
#                     print out_tmp $this->{Filter}[int(@line[$k])]," ";
#                 }
#                 print out_tmp "\n";
#             }
#             close in_tmp;
#             close out_tmp;
#             return;
#         }
#         ############################################################################
#         #01.09.2005 13:04
#         package main;
#         use strict;
#         my %inp=("Query",@ARGV[0], "DBFile",@ARGV[1] ,"RESDIR",@ARGV[2], "INDEXFILE",@ARGV[3], "REMOTEDO",@ARGV[4]);
#
#         open IN, "< $inp{Query}.res"                        or die " Could not open file $inp{Query}.res ";
#         open OUT, "> $inp{Query}.result";
#         my $cvt = CVT_IND::new($inp{INDEXFILE});
#         while ( <IN> ) {
#             my @line = split " ",$_;
#             next                                        if (int(@line)<2);
#             open TMP, "> $inp{Query}.tmp.tmp";
#             for ( my $it = 1;$it<int(@line);$it+=2 ) {
#                 print TMP int(@line[$it])," ";
#             }
#             close TMP;
#             $cvt->Do($inp{Query}.".tmp.tmp",$inp{Query}.".tmp.tmp.a");
#             system("cat ".$inp{Query}.".tmp.tmp.a |".$inp{REMOTEDO}." ".$inp{RESDIR}."/arcview ".$inp{DBFile}." -iu >".$inp{Query}.".tmp.tmp1");
#             open TMP, "< $inp{Query}.tmp.tmp1";
#             while ( <TMP> ) {
#                 chomp;
#                 print OUT $_,"\t",@line[0],"\n";
# #print $_,"\t",@line[0],"\n";
#             }
#             close TMP;
#         }
#         close OUT;
#     }' "$Query" "$DBFile" "$RESDIR"  "$INDEXFILE" "$REMOTEDO"
#     awk '{print $9,"\t",$10}' $Query.result > $Query.fin
#     rm $Query.res $Query.result $Query.tmp*
#     mv $Query.fin $Query.result
# }
# # GenerateOutputCodesMulti() {
# #     if [ -e $1.result.sum ]; then rm $1.result.sum ;fi
# #     for aa in $(ls /Berkanavt/database/yandex/ | grep oldbd00 | awk '{print substr($1,0,8)}' | sort | uniq); do
# #         GenerateOutputCodes $1 /Berkanavt/database/yandex/$aa
# #         cat $1.result >> $1.result.sum
# #     done
# #     mv $1.result.sum $1.result
# # }
#$RESDIR/$Query.result

#     cat $RESDIR/$Query   | $RESDIR/porno2 -y $DBFile -q /dev/stdin -o /dev/stdout -m |
#         awk -F "\t" '{print $0 }' |
#         awk '{ for (k=2;k<=NF;k+=2) { print $1 > "'$TMPDIR/$Query'.Code";print $k > "'$TMPDIR/$Query'.Doc" }}'
#
#     cat $TMPDIR/$Query.Doc | perl -e '{
#         use strict;
#         sub ReadFilter{
#             my ($cvt_name) = @_;
#             no strict "subs";
#             if (not open(F, "< $cvt_name")){
#                 print STDERR "file not found ",$cvt_name,"\n";
#                 return;
#             }
#             binmode(F);
#             my $buf;
#             my $num = 0;
#             my @cvt;
#             while(sysread(F, $buf, 4)){
#                 @cvt[$num] = unpack ("I1", $buf);
#                 $num++;
#             }
#             close(F);
#             return @cvt;
#         }
#         my @cvt = ReadFilter(@ARGV[0]);
#         while (<STDIN>){
#             chomp;
#             print @cvt[int($_)],"\n";
#         }
#     }' "$INDEXFILE" | $RESDIR/arcview $DBFile -iu | awk '{print $9}' > $TMPDIR/$Query.Cite
#     paste $TMPDIR/$Query.Cite $TMPDIR/$Query.Code >$RESDIR/$Query.result
