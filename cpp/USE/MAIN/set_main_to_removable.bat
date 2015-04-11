set extensions=*.cpp *.h *.f *.mak *.c *.cfg
if "%1"=="" goto zero
call test_diff_plus C:\ALEX\CPP H:\gasd\CPP %1  %extensions%
goto next
:zero
call test_diff_plus  C:\ALEX\CPP H:\gasd\CPP   %extensions%
:next

call test_diff