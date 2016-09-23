#-------------------
# Depend on GTEST_SOURCE option, compile googletest source or find the pre-compiled lib.
#-------------------
#enable_testing()

#-------------------
# Googletest as subproject
# If GTEST_SOURCE_DIR does not exist, by default cmake think the source dir is under the same directory with miniSTL project. 
#-------------------
if(GTEST_SOURCE)
    if (NOT GTEST_SOURCE_DIR)
        set(GTEST_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../googletest" CACHE PATH "GoogleTest source path")
    endif()
    message(STATUS "${GTEST_SOURCE_DIR}")
    add_subdirectory("${GTEST_SOURCE_DIR}" "gtest")
else()
    find_package(GTest REQUIRED)
    add_library(gtest ALIAS GTest::GTest)
    add_library(gtest_main ALIAS GTest::Main)
endif(GTEST_SOURCE)

set(testSOURCE 
"shared_ptr_case.cpp"
)

#-------------------
# testLib pack gtest, gtestmain, miniSTL and necessary include directory.
#-------------------
#add_library(testLib) 
#target_link_libraries(testLib PUBLIC gtest PUBLIC gtestmain PUBLIC miniSTL)
#target_include_directories(shared_ptr_test PUBLIC ${GTEST_INCLUDE_DIR} PUBLIC ".")

#-------------------
# Add test executable and link test libraries
#-------------------
add_executable(shared_ptr_test "shared_ptr_case.cpp")
target_link_libraries(shared_ptr_test gtest gtest_main miniSTL)
