:: Debug Update.cmd
:: 11/1/2012 jichi
@echo off
setlocal

cd /d %~dp0

if exist Update/Scripts/update.cmd call Update/Scripts/update.cmd

echo.
pause

:: EOF
