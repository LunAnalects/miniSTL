#ifndef MINISTL_TEST_VECTOR_CASE_HPP
#define MINISTL_TEST_VECTOR_CASE_HPP

#include "gtest/gtest.h"
#include "test_utility.hpp"
#include <string>
#include <numeric>

//ministl
#include "vector.hpp"
#include "unique_ptr.hpp"

//STL
#include <vector>
TEST(VECTOR_CASE, COTOR)
{
	std::vector<int> stlv(23);
	ministl::vector<int> miniv(23);
	EXPECT_EQ(stlv.size(), miniv.size());


}

TEST(VECTOR_CASE, ACCESS)
{
		{
			using std::vector;
			std::cout << "STD" << std::endl;

			int ai = 1;
			vector<int> a(3);
			std::cout << a.capacity() << std::endl;;
			decltype(a)::pointer ap = &ai;
			vector<int> twoint(23, 3);
		}

		{
			using ministl::vector;
			std::cout << "MINISTL" << std::endl;

			int ai = 1;
			vector<int> a(3);
			std::cout << a.capacity() << std::endl;;
			decltype(a)::pointer ap = &ai;;
			

			vector<int> twoint(size_t(23), 3);
			std::iota(twoint.begin(), twoint.end(), 0);

			ContainerOut(twoint);

			Cout3("itoa", twoint[0], twoint[2], twoint[22]);

			auto it = twoint.erase(twoint.begin() + 2);
			Cout3("erase one", twoint.size(), twoint.capacity(),it-twoint.begin());
			
			ContainerOut(twoint);


			twoint.erase(twoint.begin(), twoint.end());
			Cout3("erase range", twoint.size(), twoint.capacity());
		}
}

TEST(VECTOR_CASR, COPYCONTROL)
{
	//std::vector<HasPtr<int>> stdv33(33);
	{
		using ministl::vector;
		TICKDEFINE;
		TICKBEGIN;
		vector<double> v33(330);
		vector<double> v33_2(v33);
		TICKEND(mini copyconstructor\n);
		Cout3("v33:", v33.size(), v33_2.size(), v33[350]);
	}

	{
		using std::vector;
		TICKDEFINE;
		TICKBEGIN;
		vector<double> v33(330);
		vector<double> v33_2(v33);
		TICKEND(STD copyconstructor\n);
	}
}
#endif
