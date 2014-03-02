@echo off
:: coding: sjis, ff=dos
:: update.cmd
:: 11/12/2012 jichi
::
:: Note for wine:
:: - Leading redirection to nul is not supported
:: - :: lines are not hidden
setlocal
cd /d %~dp0
cd /d ../..

if not exist .hgignore (
  echo WARNING: Unknown library repository.
  exit /b 1
)

if exist Update/Scripts/initenv.cmd call Update/Scripts/initenv.cmd

::            1         2         3         4         5         6         7
echo ----------------------------------------------------------------------
echo                          Updating Library ...
echo ----------------------------------------------------------------------

tskill python 2>nul
tskill pythonw 2>nul
tskill 7za 2>nul
tskill hg 2>nul
::tskill wget

echo hg pull -f ^&^& hg up -C default
hg %HG_OPT% pull -f && hg %HG_OPT% up -C default

if exist Update/Scripts/upgrade.cmd call Update/Scripts/upgrade.cmd

:: EOF
