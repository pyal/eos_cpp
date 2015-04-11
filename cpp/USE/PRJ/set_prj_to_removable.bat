set extensions=*.dsp *.dsw *.ncb *.opt *.plg *.cfg
if "%1"=="" goto zero
call test_diff_plus C:\ALEX\VC H:\GASD\PRJ %1  %extensions% 
goto next
:zero
call test_diff_plus  C:\ALEX\VC H:\GASD\PRJ   %extensions%
:next

call test_diff