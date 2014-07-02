# 3/19/2014
# https://github.com/wbotelhos/raty
# wbotelhos.com/raty

WGET = wget
ifeq ($(OS),Windows_NT)
  #YUI = yuicompressor.cmd
  CLOSURE = closure.cmd
else
  #YUI = yuicompressor
  CLOSURE = closure
  #CLOSURE = closure --compilation_level ADVANCED_OPTIMIZATIONS
endif

all: $(wildcard *.min.js) $(wildcard images/*.png)

%.min.js: %.js
	$(CLOSURE) --js $^ --js_output_file $@

#REPO = http://raw.githubusercontent.com/wbotelhos/raty/master/lib
REPO = http://rawgithub.com/wbotelhos/raty/master/lib
jquery.raty.js:
	$(WGET) -O $@ $(REPO)/$@

images/%.png:
	$(WGET) -O $@ $(REPO)/$@

#fonts/raty.%:
#	$(WGET) -O $@ $(REPO)/$@

# EOF
