# This file is part of the dune-pymor project:
#   https://github.com/pymor/dune-pymor
# Copyright Holders: Stephan Rave, Felix Schindler
# License: BSD 2-Clause License (http://opensource.org/licenses/BSD-2-Clause)

project(dune-pymor CXX)

# local environment
set( ENV{PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../local/bin:$ENV{PATH}" )
set( ENV{LD_LIBRARY_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../local/lib:$ENV{LD_LIBRARY_PATH}" )
set( ENV{PKG_CONFIG_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/../local/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}" )

# cmake specific
cmake_minimum_required( VERSION 2.8 )

set( CMAKE_COLOR_MAKEFILE ON)

# guess dune-common build dir
if(NOT (dune-common_DIR OR dune-common_ROOT OR
        "${CMAKE_PREFIX_PATH}" MATCHES ".*dune-common.*"))
    string(REPLACE  ${CMAKE_PROJECT_NAME} dune-common dune-common_DIR
    ${PROJECT_BINARY_DIR})
endif()

#find dune-common and set the module path
find_package(dune-common REQUIRED)
list(APPEND CMAKE_MODULE_PATH ${dune-common_MODULE_PATH}
"${PROJECT_SOURCE_DIR}/cmake/modules")

#include the dune macros
include(DuneMacros)

# start a dune project with information from dune.module
dune_project()

# dune-stuff and dune-pymor cmake includes
list(APPEND CMAKE_MODULE_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    ${CMAKE_CURRENT_SOURCE_DIR}/../dune-stuff/cmake)

include(DuneUtils)
include(PymorUtils)

link_directories( "/usr/lib" ${CCGNU_LIBRARY_DIRS} )

# header
file( GLOB_RECURSE pymor "${CMAKE_CURRENT_SOURCE_DIR}/*.hh" )
set( COMMON_HEADER ${pymor} ${DUNE_HEADERS} )
# add header of dependent modules for header listing
foreach(_mod ${ALL_DEPENDENCIES})
        file(GLOB_RECURSE HEADER_LIST "${CMAKE_CURRENT_SOURCE_DIR}/../${_mod}/*.hh")
        list(APPEND COMMON_HEADER ${HEADER_LIST})
endforeach(_mod)
set_source_files_properties( ${COMMON_HEADER} PROPERTIES HEADER_FILE_ONLY 1 )


#disable most warnings from dependent modules
foreach(_mod ${ALL_DEPENDENCIES})
  dune_module_to_uppercase(_upper_case "${_mod}")
  if(${_mod}_INCLUDE_DIRS)
    foreach( _idir ${${_mod}_INCLUDE_DIRS} )
      add_definitions("-isystem ${_idir}")
    endforeach( _idir )
  endif(${_mod}_INCLUDE_DIRS)
endforeach(_mod DEPENDENCIES)

add_subdirectory(dune)

# libs
set(COMMON_LIBS
    dunepymor
    ${DUNE_DEFAULT_LIBS}
)

add_subdirectory(examples EXCLUDE_FROM_ALL)
add_subdirectory(test EXCLUDE_FROM_ALL)

finalize_dune_project(GENERATE_CONFIG_H_CMAKE)
