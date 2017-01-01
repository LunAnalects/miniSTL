#ifndef MINISTL_UTILITY_STL
#define MINISTL_UTILITY_STL

#include "memory.hpp"
#include "algorithm.hpp"
#include "type_traits.hpp"

namespace ministl
{
	//This implementation of move() is from C++Primer.
	template <typename T>
	typename remove_reference<T>::value_type&& move(T&& t)
	{
		return static_cast<typename remove_reference<T>::value_type &&>(t);
	}

	template<typename T>
	void swap(T& lhs, T& rhs)
	{
		auto temp(ministl::move(lhs));
		lhs = ministl::move(rhs);
		rhs = ministl::move(temp);
	}

	template<typename T,size_t N>
	void swap(T(&lhs)[N], T(&rhs)[N])
	{
		ministl::swap_ranges(lhs, lhs + N, rhs);
	}
} //ministl


#endif //MINISTL_UTILITY_STL
