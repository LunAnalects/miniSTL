
# miniSTL

## Build and Test
miniSTL uses Googletest as test framework and CMake as make utility. 
In CMake configuration, you can choose GTEST_SOURCE to build googletest from source which is googletest's official advise(to do so, keep miniSTL and googletest source in the same directory), or you can let cmake find your installed googletest. 

##In Progress：
* vector.hpp          : 80%       not fully tested.
* unique_ptr.hpp      : 80%       not fully tested.
* shared_ptr.hpp      : 60%       not tested yet.
* basic_string.hpp    : 30%       not tested yet.
