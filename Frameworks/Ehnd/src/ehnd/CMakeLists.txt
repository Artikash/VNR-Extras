# CMakeList.txt
# 12/5/2015 jichi
# See: http://stackoverflow.com/questions/24872225/a-simple-example-of-using-cmake-to-build-a-windows-dll
# See: http://www.cmake.org/Wiki/BuildingWinDLL

# Source files
set(ehnd_SOURCES
  config.cpp
  dllmain.cpp
  ehnd.cpp
  filter.cpp
  hook.cpp
  log.cpp
  stdafx.cpp
  watch.cpp
)

set(ehnd_LIBRARIES
  ${Boost_REGEX_LIBRARY} # not working properly on Windows
  psapi winmm
)

set(ehnd_DEF ${CMAKE_CURRENT_LIST_DIR}/ehnd.def) # exported functions to linker

# Library targets

# Use manual def
# http://stackoverflow.com/questions/18786690/cmake-for-def-and-nodefaultlib
# http://public.kitware.com/pipermail/paraview/2010-September/018752.html
set(CMAKE_SHARED_LINKER_FLAGS
  "${CMAKE_SHARED_LINKER_FLAGS} /DEF:${ehnd_DEF}" #/NODEFAULTLIB
)

set(LIB_TYPE SHARED)
add_library(ehnd ${LIB_TYPE} ${ehnd_SOURCES})
target_link_libraries(ehnd ${ehnd_LIBRARIES})

install(TARGETS ehnd RUNTIME
  DESTINATION bin
  CONFIGURATIONS Release
)

# EOF
