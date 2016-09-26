cmake_minimum_required(VERSION 3.1)
project(miniSTL)
SET(CMAKE_cxx_CREATE_STATIC_LIBRARY TRUE)

#-------------------
# Options
#-------------------
option(miniSTL_TEST "Enable test by googletest" OFF)
option(GTEST_SOURCE "Build googletest from source" OFF) 
#Export compiler commands, only valid in make and ninja
option(CMAKE_EXPORT_COMPILE_COMMANDS "Export json file containing compiler commands" ON)


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
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")

#Keep the same with Googletest in VS
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

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
    "${miniSTL_SOURCE_DIR}/internal/pointer_trait.hpp"
    "${miniSTL_SOURCE_DIR}/internal/shared_ptr.hpp"
    "${miniSTL_SOURCE_DIR}/internal/temp.h"
    "${miniSTL_SOURCE_DIR}/internal/types.hpp"
    "${miniSTL_SOURCE_DIR}/internal/unique_ptr.hpp"
    "${miniSTL_SOURCE_DIR}/internal/weak_ptr.hpp"
    "${miniSTL_SOURCE_DIR}/internal/enable_shared.hpp"
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

