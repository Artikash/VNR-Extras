:: Debug Update.cmd
:: 11/1/2012 jichi
@echo off
setlocal

cd /d %~dp0/..

if exist Library/Update/Scripts/update.cmd call Library/Update/Scripts/update.cmd

echo.
pause

:: EOF
