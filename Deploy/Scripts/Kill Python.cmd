:: Kill Python.cmd
:: 9/29/2013 jichi
@echo off
setlocal

title Kill Python

echo.
echo tskill python
tskill python

echo.
echo tskill pythonw
tskill pythonw

::echo.
::echo tskill hg
::tskill hg

echo.
pause

:: EOF
