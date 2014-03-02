#!/bin/bash
# 12/17/2012 jichi
ME="$(basename "$0" .sh)"

cd "$(dirname "$0")"

MACPORTS_HOME=/opt/local
DARWINE_HOME=/Applications/Wine.app/Contents/Resources
export PATH=$PATH:$MACPORTS_HOME/bin:$DARWINE_HOME/bin

die()
{
  >&2 echo "$@"
  exit 1
}

require()
{
  local i
  for i; do
    which "$i" >/dev/null 2>&1 || \
      die "$ME: cannot find '$i' in PATH, ABORT"
  done
}
require wine
WINE=wine

HG="$(PWD)/Update/mercurial/hg.exe"
HG="$(echo "z:$HG" | sed 's,/,\\\\,g')"
#HG="$WINE $HG"    # does not work when there are spaces in HG orz

# Use native hg if find hg in PATH
#which hg >/dev/null 2>&1 && HG=hg

_hg() {
  if $(which hg >/dev/null 2>&1); then
    echo "$ME: using native hg in the PATH"
    hg "$@"
  else
    $WINE "$HG" "$@"
  fi
}

HG_OPT="-v --debug"

REPOS="\
. \
Update \
Dictionaries \
Fonts \
Frameworks/Python \
Frameworks/Qt \
Frameworks/MeCab \
Frameworks/NTLEA \
Frameworks/LocaleSwitch \
Frameworks/EB \
Frameworks/AJAX \
Frameworks/Sakura \
"

#die()
#{
#  >&2 echo "$@"
#  exit -1
#}
#
#require()
#{
#  local i
#  for i; do
#    which "$i" >/dev/null 2>&1 || \
#      die "$ME: cannot find '$i' in PATH, ABORT"
#  done
#}
#require hg

test -e .org.sakuradite.hg.stream-base || die "unknown hg repository"

for f in `echo $REPOS`; do
  if [ -x "$f" ]; then
    pushd "$f"
    echo hg pullup: `pwd`
    _hg $HG_OPT pull && \
      _hg $HG_OPT up
    popd
  fi
done

test -x Deployment && rsync -av Deployment/* ..

# EOF
