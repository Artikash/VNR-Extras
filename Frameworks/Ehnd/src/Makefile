# Makefile
# 4/3/2015 jichi
.PHONY: all clean compile config

BUILD_DIR = build
PREFIX_DIR = install

# Temporary files to put build files
ifeq ($(OS),Windows_NT)
  BOOST_HOME = z:/local/windows/developer/boost/build-12
else
  BOOST_HOME = $(HOME)/opt/boost
endif

# Tool chain
CMAKE_GENERATOR = Visual Studio 12 2013

all: compile
	@echo targets: compile clean

clean:
	rm -Rf $(BUILD_DIR)

compile: config
	cmake --build $(BUILD_DIR) \
	  --config Release \
	  --target install

$(BUILD_DIR):
	mkdir $@

config: $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake \
	  -DCMAKE_BUILD_TYPE=RELEASE \
	  -G "$(CMAKE_GENERATOR)" \
	  -DBOOST_ROOT=$(BOOST_HOME) \
	  -DCMAKE_INSTALL_PREFIX=$(PREFIX_DIR) \
	  $(CURDIR)

# EOF
