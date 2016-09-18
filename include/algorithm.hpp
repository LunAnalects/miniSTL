#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

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

	template <typename InputIterator, typename Predicate>
	bool all_of(InputIterator first, InputIterator last, Predicate pred)
	{
		for(auto iter = first; iter != last; ++iter)
		{
			if (!pred(*iter))
				return false;
		}
		return true;
	}

	template <typename InputIterator, typename Predicate>
	bool any_of(InputIterator first, InputIterator last, Predicate pred)
	{
		for (auto iter = first; iter != last; ++iter)
		{
			if (pred(*iter))
				return true;
		}
		return false;
	}

	template <typename InputIterator, typename Predicate>
	bool none_of(InputIterator first, InputIterator last, Predicate pred)
	{
		for (auto iter = first; iter != last; ++iter)
		{
			if (pred(*iter))
				return false;
		}
		return false;
	}

	template <typename InputIterator, typename UnaryFunction>
	UnaryFunction for_each(InputIterator first, InputIterator last, UnaryFunction f)
	{
		for(; first !=last; ++first)
		{
			f(*first);
		}
		return f;
	}

	template <typename InputIterator, typename T>
	typename iterator_traits<InputIterator>::difference_type
		count(InputIterator first, InputIterator last, const T &value)
	{
		typename iterator_traits<InputIterator>::difference_type counter = 0;
		for(auto iter = first; iter != last; ++iter)
		{
			if (*iter == value)
				++counter;
		}
		return counter;
	}
} //ministl


#endif //MINISTL_ALGORITHM_HPP