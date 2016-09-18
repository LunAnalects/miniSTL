#ifndef MINISTL_TEST_SHARED_PTR_CASE_HPP
#define MINISTL_TEST_SHARED_PTR_CASE_HPP

#include <string>

#include "gtest/gtest.h"
#include "test_utility.hpp"

//ministl
#include "memory.hpp"
//STL
#include <memory>

using ministl::shared_ptr;

//Helper classes
struct test1
{
	int m1 = 2;
	int m2 = 3;
};

#endif //MINISTL_TEST_SHARED_PTR_CASE_HPP