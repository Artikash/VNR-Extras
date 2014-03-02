#!/bin/sh
# 9/29/2013 jichi
ME="`basename "$0" .sh`"

echo "$ME: killall wine wineserver"
killall wine wineserver "$@"

# EOF
