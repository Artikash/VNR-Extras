# Makefile
# 8/8/2013 jichi

all: compress

WGET = wget
ifeq ($(OS),Windows_NT)
  YUI = yuicompressor.cmd
  #CLOSURE = closure.cmd
else
  YUI = yuicompressor
  #CLOSURE = closure
  #CLOSURE = closure --compilation_level ADVANCED_OPTIMIZATIONS
endif

compress: $(wildcard *.min.js)
download: $(wildcard *.js)

%.min.js: %.js
	$(YUI) -o $@ $^
	#$(YUI) --type js -o $@ $^

#%.min.js: %.js
#	$(CLOSURE) --js $^ --js_output_file $@


# https://npmjs.org/package/dateformat
#dateformat.js:
#	$(WGET) -O $@  https://raw.github.com/felixge/node-dateformat/master/lib/dateformat.js
# https://npmjs.org/package/haml
haml.js:
	$(WGET) -O $@ https://raw.github.com/creationix/haml-js/master/lib/haml.js

# EOF
