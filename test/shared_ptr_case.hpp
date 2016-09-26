#ifndef MINISTL_TEST_SHARED_PTR_CASE_HPP
#define MINISTL_TEST_SHARED_PTR_CASE_HPP

#include <string>

#include "test_utility.hpp"

//ministl
#include "memory.hpp"
//STL
#include <memory>

using ministl::shared_ptr;

//Helper classes
struct helpPrint
{
	helpPrint(int i1, int i2):m1(i1), m2(i2) {
		std::cout << "ctor: m1(" << m1 << ") m2:(" << m2 << ")" << std::endl;
	}
	helpPrint(const helpPrint&r) :m1(r.m1), m2(r.m2) {
		std::cout << "copyctor: m1(" << m1 << ") m2:(" << m2 << ")" << std::endl;
	}
	~helpPrint() {
		std::cout << "detor: m1(" << m1 << ") m2:(" << m2 << ")" << std::endl;
	}


	int m1 = 2;
	int m2 = 3;
};

#endif //MINISTL_TEST_SHARED_PTR_CASE_HPP