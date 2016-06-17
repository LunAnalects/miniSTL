#ifndef MINISTL_TEST_MEMORY_CASE_HPP
#define MINISTL_TEST_MEMORY_CASE_HPP

#include "gtest/gtest.h"
#include "test_utility.hpp"
#include <string>

//ministl
#include "memory.hpp"
#include "algorithm.hpp"
//STL
#include <vector>

TEST(MEMORY_CASE,UINI_COPY)
{
	const int len = 42;
	std::vector<int> vsource(len, 1);
	int b[len];
	ministl::uninitialized_copy_n(vsource.data(), len, b);
	for (size_t i = 0; i < vsource.size(); ++i)
	{
		EXPECT_EQ(b[i],vsource[i]);
	}
}

TEST(MEMORY_CASE, SWAP)
{
	int a = 1;
	int b = 2;
	int *p = &a, *p2 = &b;
	ministl::iter_swap(p, p2);

	int* arr1 = ArrayGen(200, 2);
	int* arr2 = ArrayGen(200, 1);
	auto pass_end = ministl::swap_ranges(arr1, arr1 + 200, arr2);
	EXPECT_EQ(arr1[20], arr2[19]);
	EXPECT_EQ(arr1[199], arr2[198]);
	EXPECT_EQ(*(pass_end - 1), arr2[199]);
}

TEST(MEMORY_CASE, SWAP2)
{
	HasPtr<int> a(23);
}

TEST(MEMORY_CASE, copy)
{
}
#endif // MINISTL_TEST_MEMORY_CASE_HPP
