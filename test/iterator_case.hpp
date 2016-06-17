#ifndef MINISTL_TEST_ITERATOR_CASE_HPP
#define MINISTL_TEST_ITERATOR_CASE_HPP
#include "gtest/gtest.h"
#include <string>
#include "test_utility.hpp"

//ministl
#include "iterator.hpp"
#include "memory.hpp"
//STL

TEST(ITERATOR_CASE,CTOR)
{

}

TEST(ITERATOR_CASE, GENERAL)
{

	ministl::iterator_traits<int*>::value_type a = 2;
}

#endif // MINISTL_TEST_ITERATOR_CASE_HPP
