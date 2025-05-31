# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\tetris_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\tetris_autogen.dir\\ParseCache.txt"
  "tetris_autogen"
  )
endif()
