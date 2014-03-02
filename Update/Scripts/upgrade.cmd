@echo off
:: coding: sjis, ff=dos
:: upgrade.cmd
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

::            1         2         3         4         5         6         7
echo.
echo ----------------------------------------------------------------------
echo                         Updating Frameworks ...
echo ----------------------------------------------------------------------

if exist Update/Scripts/initenv.cmd call Update/Scripts/initenv.cmd

::tskill python
::tskill pythonw
::tskill 7za
::tskill hg

::if not exist Library/Frameworks mkdir Library\Frameworks

if not exist Frameworks mkdir Frameworks

:: enter Frameworks
pushd Frameworks

:: Note: Use -C to clean the repo

if not exist Python/.hgignore (
  rm -Rf Python
  echo hg %HG_OPT% clone http://%REPO_HOST%/hg/vnr-python Python
  hg %HG_OPT% clone http://%REPO_IP%/hg/vnr-python Python
) else  (
  pushd Python
  echo hg pull -f ^&^& hg up -C default
  hg %HG_OPT% pull -f && hg %HG_OPT% up -C default
  popd
)

if not exist EB/.hgignore (
  rm -Rf EB
  echo hg %HG_OPT% clone http://%REPO_HOST%/hg/eb EB
  hg %HG_OPT% clone http://%REPO_IP%/hg/eb EB
) else  (
  pushd EB
  echo hg pull -f ^&^& hg up default
  hg %HG_OPT% pull -f && hg %HG_OPT% up default
  popd
)

if not exist Sakura/.hgignore (
  rm -Rf Sakura
  echo hg %HG_OPT% clone http://%REPO_HOST%/hg/vnr-sakura Sakura
  hg %HG_OPT% clone http://%REPO_IP%/hg/stream-sakura Sakura
) else  (
  pushd Sakura
  echo hg pull -f ^&^& hg up default
  hg %HG_OPT% pull -f && hg %HG_OPT% up default
  popd
)

:: leave Frameworks
popd

for %%i in (
    Update ^
    Fonts ^
    Dictionaries ^
    Frameworks\Python ^
    Frameworks\Qt ^
    Frameworks\MeCab ^
    Frameworks\NTLEA ^
    Frameworks\LocaleSwitch ^
    Frameworks\AJAX ^
    Frameworks\EB ^
    Frameworks\Sakura ^
  ) do (
  if exist %%i/__init__.cmd (
    call %%i/__init__.cmd
  )
)

::            1         2         3         4         5         6         7
echo.
echo ----------------------------------------------------------------------
echo                    Updating EDICT if out of date ...
echo ----------------------------------------------------------------------

set PATH=%CD%/Frameworks/Python;%PATH%

if exist ../Caches/Dictionaries/EDICT/edict.db (
  if exist Frameworks/Sakura/py/scripts/getedict.py (
    python -B Frameworks/Sakura/py/scripts/getedict.py
))

::            1         2         3         4         5         6         7
echo.
echo ----------------------------------------------------------------------
echo                        Repairing permissions ...
echo ----------------------------------------------------------------------

:: Remove *.pyc and *.pyo. Ignore errors.
::echo.
::echo update: removing python byte code ...
::del /s /q /f *.pyc *.pyo
::
::echo update: touching python source code ...
::for /f "tokens=* delims=|" %%i in ('dir /s /b *.py') do touch "%%~i"

rm -f Frameworks/Python/GPS.txt

for %%i in (
    . ^
    Caches ^
    Scripts ^
  ) do (
  if exist Deploy/%%i (
    if not exist ../%%i mkdir ..\%%i
    >nul 2>nul copy /y Deploy\%%i\* ..\%%i\

    if exist ../%%i (
      pushd ..\%%i

      attrib +r +s .
      if exist desktop.ini attrib +h +s desktop.ini

      if exist icon.ico attrib +h icon.ico

      popd
)))

attrib +h *Mac*
if exist "VNR for Mac.app" attrib +h "VNR for Mac.app"

attrib +r +s .
if exist desktop.ini attrib +h +s desktop.ini
if exist icon.ico attrib +h icon.ico
if exist .hg attrib +h .hg
if exist .hgignore attrib +h .hgignore

if exist ../Scripts copy /y *.cmd ..\Scripts\ >nul

::            1         2         3         4         5         6         7
echo.
echo ----------------------------------------------------------------------
echo                           Update complete!
echo ----------------------------------------------------------------------

:: leave Library
cd ..
::if exist Changes.txt start notepad Changes.txt
if exist Changes.txt explorer Changes.txt

:: EOF