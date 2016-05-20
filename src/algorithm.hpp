#ifndef MINISTL_ALGORITHM_HPP
#define MINISTL_ALGORITHM_HPP
#include "utility.hpp"

namespace ministl
{


	template<typename ForwardIterator1, typename ForwardIterator2>
	void iter_swap(ForwardIterator1 a, ForwardIterator2 b)
	{
		using ministl::swap;
		swap(*a, *b);
	}

	template<typename ForwardIterator1, typename ForwardIterator2>
	ForwardIterator2 swap_ranges(ForwardIterator1 begin, ForwardIterator1 end, ForwardIterator2 dest)
	{
		for (; begin != end; ++begin, ++dest)
		{
			iter_swap(begin, dest);
		}
		return dest;
	}

} //ministl


#endif //MINISTL_ALGORITHM_HPP