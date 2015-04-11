if "%1" == "" exit 1
dir /AD /b >!
getstr ! copy #1 %1
cd %1 
if not %errorlevel% == 0 exit 1
rm ideal* 