cmake_minimum_required(VERSION 3.4)

project(PPBS)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${Python3_INCLUDE_DIRS})

set(BSIC_EXECUTABLE pbsic)
add_custom_command(
  OUTPUT pxtypes-generated.hh
  COMMAND ${BSIC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/pxtypes.bsi ${CMAKE_CURRENT_BINARY_DIR}/pxtypes-generated.hh
  )

add_custom_command(
  OUTPUT ppbs.cc
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/ppbs-gen.pl px4 ${CMAKE_CURRENT_SOURCE_DIR}/pxtypes.hh ${CMAKE_CURRENT_SOURCE_DIR}/pxtypes.bsi EventIndex,uint32 > ${CMAKE_CURRENT_BINARY_DIR}/ppbs.cc
  )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_subdirectory(pybind11)
pybind11_add_module(ppbs-px4 ppbs.cc pxtypes-generated.hh MODULE)
target_link_libraries(ppbs-px4 PRIVATE pbsf_s)
