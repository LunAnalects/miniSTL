#ifndef MINISTL_TEST_SHARED_PTR_CASE_HPP
#define MINISTL_TEST_SHARED_PTR_CASE_HPP

#include "gtest/gtest.h"
#include "test_utility.hpp"
#include <string>

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

TEST(SHARED_PTR_CASE, CTOR)
{
	shared_ptr<int> intsp(new int(3));
	shared_ptr<test1> test1sp(new test1);

}

TEST(SHARED_PTR_CASE, CTOR2)
{

}



#endif //MINISTL_TEST_SHARED_PTR_CASE_HPP