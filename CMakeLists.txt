cmake_minimum_required(VERSION 3.1)
project(miniSTL)

#-------------------
# Options
#-------------------
option(miniSTL_TEST "Enable test by googletest" OFF)
option(GTEST_SOURCE "Build googletest from source" OFF) 

#-------------------
# Set variables
#-------------------
set(miniSTL_INCLUDE_DIR "include")
set(miniSTL_SOURCE_DIR "source")

#-------------------
# Include
#-------------------
include_directories("${miniSTL_INCLUDE_DIR}")
include_directories("${miniSTL_SOURCE_DIR}")
include_directories("${miniSTL_SOURCE_DIR}/internal")
#-------------------
# Compiler Options
#-------------------
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wall -Wextra -Werror")

#-------------------
# Set *.cpp *.cc source files. so far miniSTL is header-only.
#-------------------
set (miniSTL_HEADERS 
    "${miniSTL_INCLUDE_DIR}/algorithm.hpp"
    "${miniSTL_INCLUDE_DIR}/allocator.hpp"
    "${miniSTL_INCLUDE_DIR}/functional.hpp"
    "${miniSTL_INCLUDE_DIR}/iterator.hpp"
    "${miniSTL_INCLUDE_DIR}/memory.hpp"
    "${miniSTL_INCLUDE_DIR}/type_traits.hpp"
    "${miniSTL_INCLUDE_DIR}/utility.hpp"
    "${miniSTL_INCLUDE_DIR}/vector.hpp"
    "${miniSTL_SOURCE_DIR}/internal/basic_string.hpp"
    "${miniSTL_SOURCE_DIR}/internal/pointer.hpp"
    "${miniSTL_SOURCE_DIR}/internal/temp.h"
    "${miniSTL_SOURCE_DIR}/internal/types.hpp"
    "${miniSTL_SOURCE_DIR}/internal/unique_ptr.hpp"
    "${miniSTL_SOURCE_DIR}/internal/unique_ptr_template.hpp"
    "${miniSTL_SOURCE_DIR}/internal/weak_ptr.hpp"
    )
set (miniSTL_SOURCES )

#-------------------
# Library target, target include.
#-------------------
add_library(miniSTL ${miniSTL_HEADERS})
#The next line is to fix cmake link language error in Windows and VS2015.
set_target_properties(miniSTL PROPERTIES  LINKER_LANGUAGE cxx)

#--------
#Enable tests, see test/CMakeLists.txt.
#--------
if(miniSTL_TEST)
    add_subdirectory("test")
endif(miniSTL_TEST)

