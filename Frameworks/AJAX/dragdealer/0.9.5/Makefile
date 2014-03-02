# Makefile
# 8/8/2013 jichi

# http://code.google.com/p/dragdealer/
DRAGDEALER_URL = https://dragdealer.googlecode.com/svn/tags/0.9.5/dragdealer.js

all: compress

WGET = wget
ifeq ($(OS),Windows_NT)
  CLOSURE = closure.cmd
else
  #CLOSURE = closure --compilation_level ADVANCED_OPTIMIZATIONS
  CLOSURE = closure
endif

compress: $(wildcard *.min.js)
download: $(wildcard *.js)

dragdealer.js:
	$(WGET) -O $@ $(DRAGDEALER_URL)

%.min.js: %.js
	$(CLOSURE) --js $^ --js_output_file $@

# EOF
