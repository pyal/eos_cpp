for aa in $(ls resource/*) ; do
	echo bash ./res_util.sh -a test_urs_resource.rc $aa 
done
