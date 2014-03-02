:: Debug Update.cmd
:: 11/1/2012 jichi
@echo off
setlocal

cd /d %~dp0

title Debug Update
::color 8f

if exist Scripts/initenv.cmd call Scripts/initenv.cmd

set HG_OPT=-v --debug

echo hg pull -f ^&^& hg up -C default
hg %HG_OPT% pull -f && hg %HG_OPT% up -C default

if exist Scripts/safeupdate.cmd call Scripts/safeupdate.cmd

echo.
pause

:: EOF
