# Makefile
# 8/8/2013 jichi
include ../../env.mk

all: compress

compress: $(wildcard *.min.js)
download: $(wildcard *.js)

%.min.js: %.js
	make .closure OUT=$@ IN=$^

# http://code.google.com/p/dragdealer/
dragdealer.js:
	make .wget OUT=$@ IN=https://dragdealer.googlecode.com/svn/tags/0.9.5/dragdealer.js

# EOF
