#ifndef MINISTL_TEST_UNIQUE_PTR_CASE_HPP
#define MINISTL_TEST_UNIQUE_PTR_CASE_HPP

#include "gtest/gtest.h"
#include <string>

//ministl
#include "unique_ptr.hpp"

//STL
#include <memory>

TEST(UNIQUE_PTR_CASE, CTOR)
{
	ministl::unique_ptr<int> ptr(new int(23));
	
	//no specialiazation for T[]
	//ministl::unique_ptr<int[]> array_ptr(new int[24]);

	ministl::unique_ptr<int> array_ptr(new int[33]);


}

#endif