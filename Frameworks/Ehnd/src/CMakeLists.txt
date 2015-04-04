# CMakeList.txt
# 12/5/2015 jichi
# Windows only
cmake_minimum_required(VERSION 2.8.3)

# Show compile/link commands
# https://sidvind.com/wiki/CMake/Verbose_output
# http://stackoverflow.com/questions/6623778/cmake-linking-to-boost-error-lnk2005

# Enable C++11
# http://stackoverflow.com/questions/10851247/how-to-activate-c-11-in-cmake
#add_definitions(-std=c++11)
#add_definitions(-std=c++0x)
#add_definitions(-stdlib=libc++)

# Windows definitions
add_definitions(
  # Force UNICODE on Windows
  -DUNICODE -D_UNICODE
)

# Boost
# http://stackoverflow.com/questions/6646405/how-do-you-add-boost-libraries-in-cmakelists-txt
#set(Boost_DEBUG ON) # show link/compile commands
# Always use shared mt boost
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS regex)

add_definitions(
  -DBOOST_ALL_NO_LIB 	# Disable automatically linking with boost silently
  -DBOOST_ALL_DYN_LINK	# Always do dynamic linking
)

include_directories(${Boost_INCLUDE_DIRS}) # always enable boost headers
#link_libraries(${Boost_LIBRARIES}) # deprecated
#link_directories(Z:/Local/Windows/Developer/boost/build-12/lib)

#set(BOOST_REGEX_LIBRARY boost_regex-mt)

## Sub-directories
add_subdirectory(ehnd)

# EOF
