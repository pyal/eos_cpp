
for aa in $(ls -d *) ; do
	if  [ -d $aa ] ; then
		if ! [ -s $aa.txt ] ; then
			cat >$aa.txt <<EOF
Cal              Exp                  Lambda  Shift   Opacity  Calibration
                                      640     100     0.815    0
                                      Up      -5      0.815    0
                                      Um      -5      0.815    0
                                      R       -5      0.815    0
                                      Si      100     0.815    0
                                      Dfd     100     0.815    0
$(cd $aa;ls *)
EOF
		fi
	fi
done

