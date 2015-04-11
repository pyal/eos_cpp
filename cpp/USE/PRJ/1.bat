set extensions=*.dsp *.dsw *.ncb *.opt *.plg *.cfg
if "%1"=="" goto zero
call test_diff_plus g:\vc C:\ALEX\VC %1 %extensions%
goto next
:zero
call test_diff_plus g:\vc C:\ALEX\VC  %extensions%
:next
call test_diff_plus