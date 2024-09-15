# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "MinSizeRel")
  file(REMOVE_RECURSE
  "CMakeFiles\\chess_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\chess_autogen.dir\\ParseCache.txt"
  "chess_autogen"
  )
endif()
