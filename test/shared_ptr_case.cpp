#include "shared_ptr_case.hpp"
#include "gtest/gtest.h"

TEST(SHARED_PTR_CASE, CTOR)
{
	shared_ptr<int> intsp(new int(3));
	shared_ptr<helpPrint> test1sp(new helpPrint(1,2));
}
