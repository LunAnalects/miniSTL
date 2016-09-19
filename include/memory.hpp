#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef MINISTL_MEMORY_HPP
#define MINISTL_MEMORY_HPP

#include <type_traits>
#include "types.hpp"
#include "internal/pointer.hpp"
#include "unique_ptr.hpp"
#include "shared_ptr.hpp"
#include "weak_ptr.hpp"
#include "iterator.hpp"
#include "utility.hpp"

namespace ministl
{
	template<typename T>
	inline void *get_temporary_buffer(ptrdiff_t n)
	{
		return operator new(sizeof(T) * n);
	}

	inline void return_temporary_buffer(void *p)
	{
		delete[] p;
	}
	
	template<typename ForwardIterator>
	void destroy(ForwardIterator it1, ForwardIterator it2)
	{
		using value_type = typename iterator_traits<ForwardIterator>::value_type;
		for (; it1 != it2; ++it1)
		{
			it1->~value_type();
		}
	}

	template<typename InputIterator, 
		typename Size, 
		typename ForwardIterator>
		ForwardIterator uninitialized_move_n (InputIterator first, Size n, ForwardIterator result)
	{

		for (; n >0; ++first, --n, ++result)
		{
			using value_type = typename iterator_traits<InputIterator>::value_type;
			::new(static_cast<void*>(&*result))
				value_type(ministl::move(*first));
		}
		return result;
	}

	template <typename InputIterator, 
	typename ForwardIterator>
	ForwardIterator uninitialized_move(InputIterator first, InputIterator last, ForwardIterator result)
	{
		return uninitialized_move_n(first, last - first, result);
	}

	template<typename InputIterator,
		typename Size,
		typename ForwardIterator>
		ForwardIterator uninitialized_copy_n(InputIterator first, Size n, ForwardIterator result)
	{
		auto original_result = result;
		for (; n >0; ++first, --n, ++result)
		{
			typedef typename iterator_traits<ForwardIterator>::value_type value_type;
			::new(static_cast<void*>(&*result)) value_type(*first);
		}
		return original_result;
	}

	template <typename InputIterator,
		typename ForwardIterator>
		ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
	{
		return uninitialized_copy_n(first, last - first, result);
	}

}//ministl


#endif //MINISTL_MEMORY_HPP