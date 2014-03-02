:: coding: sjis, ff=dos
:: 12/17/2012 jichi
@echo off
setlocal
cd /d %~dp0
cd ../..
SEt HG_OPT=-v --debug

if not exist .hgignore exit /b 1

for %%i in (
    . ^
    Frameworks\Python ^
    Frameworks\EB ^
    Frameworks\Sakura ^
  ) do (
  if exist %%i (
    pushd %%i
    echo hg pullup: %%i
    hg %HG_OPT% pull && hg up
    popd
  )
)

:: EOF
