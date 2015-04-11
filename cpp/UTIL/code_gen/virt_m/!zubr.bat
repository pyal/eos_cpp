@echo off
rem echo %CD%
echo f %~f0
echo p %~p0
echo d %~d0
%~d0
cd %~d0%~p0
rem zubr -vd -Cmain.c main.y
