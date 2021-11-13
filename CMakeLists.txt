cmake_minimum_required(VERSION 3.4)

project(PPBS)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Python3_INCLUDE_DIRS})

execute_process(
  COMMAND python3-config --extension-suffix
  OUTPUT_VARIABLE _modsuffix
)
string(REPLACE "\n" "" _modsuffix ${_modsuffix})
set(modname "ppbs${_modsuffix}")
message(STATUS ${modname})
add_library(ppbs SHARED ppbs.cc pxtypes-generated.hh)
set_target_properties(ppbs PROPERTIES
  PREFIX ""
  SUFFIX ${_modsuffix}
  )
target_link_libraries(ppbs pbsf_s)
