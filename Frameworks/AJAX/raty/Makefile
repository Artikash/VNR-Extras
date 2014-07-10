# Makefile
# 3/19/2014
# https://github.com/wbotelhos/raty
# http://wbotelhos.com/raty
include ../env.mk

#REPO = http://raw.githubusercontent.com/wbotelhos/raty/master/lib
REPO = http://rawgithub.com/wbotelhos/raty/master/lib

all: $(wildcard *.min.js) $(wildcard images/*.png)

%.min.js: %.js
	$(CLOSURE) --js $^ --js_output_file $@

jquery.raty.js:
	make .wget OUT=$@ IN=$(REPO)/$@

images/%.png:
	make .wget OUT=$@ IN=$(REPO)/$@

#fonts/raty.%:
#	make .wget OUT=$@ IN=$(REPO)/$@

# EOF
