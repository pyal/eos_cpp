set extensions=*.cpp *.h *.f *.mak *.c *.cfg
if "%1"=="" goto zero
call test_diff_plus H:\gasd\CPP C:\ALEX\CPP %1  %extensions%
goto next
:zero
call test_diff_plus  H:\gasd\CPP C:\ALEX\CPP  %extensions%
:next
call test_diff_plus