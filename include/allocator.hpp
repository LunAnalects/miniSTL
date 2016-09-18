#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef MINISTL_ALLOCATOR_HPP
#define MINISTL_ALLOCATOR_HPP

#include "types.hpp"
#include <stdlib.h>
#include <forward_list>

namespace ministl
{
	//TODO allocator_tarits
	template <typename T> 
	class allocator
	{
	public:
		using this_type			= allocator<T>;
		using value_type		= T;
		using pointer			= T *;
		using reference			= T &;
		using const_reference	= const T &;
		using const_pointer		= const T *;
		using size_type			= size_t;
		using difference_type	= ptrdiff_t;


	public:

		allocator() = default;
		allocator(const this_type &)
		{
			
		}
		this_type& operator=(const this_type &)
		{

		}
		~allocator() = default;

		//
		pointer address(reference x) const noexcept
		{
			return &x;
		}
		const_pointer address(const_reference x) const noexcept
		{
			return &x;
		}

		//TODO different implementation is needed. the original just calls malloc() and free()
		//return NULL when memory alloc failed.
		pointer allocate(size_type n, allocator<void>::const_pointer hint = nullptr)
		{
			size_type userSize = n * sizeof(value_type);
			pointer allocated_ptr = NULL; //use NULL because of malloc() 
			if(userSize >0 && userSize < static_cast<size_t>(-1) )
				allocated_ptr = static_cast<pointer>(malloc( n * sizeof(value_type) ));
			return allocated_ptr;
		}

		void deallocate(pointer p)
		{
			free(p);
		}

		void destruct(pointer p)
		{
			p->~value_type();
		}
		void destruct(pointer p, size_type n)
		{
			for (; n > 0; --n,++p)
			{
				p->~value_type();
			}
		}
		void destruct(pointer begin, pointer end)
		{
			size_type n = end - begin;
			destruct(begin, n);
		}

		template<typename...Args>
		void construct(pointer p,Args&&... args)
		{
			::new(static_cast<void*>(p)) value_type(std::forward<Args>(args)...);
		}

		size_type max_size() const noexcept
		{
			//implemetation in VC++ STL
			return (static_cast<size_t>(-1ll) / sizeof(value_type));
		}
		/*
		//in C++98:	void construct ( pointer p, const_reference val );
		//in C++11: template <class U, class... Args>
		//			void construct(U* p, Args&&... args);
		*/

	private:


	};

	template <> 
	class allocator<void>
	{
	public:
		using this_type			= allocator<void>;
		using pointer			= void *;
		using const_pointer		= const void *;
		using size_type			= size_t;
		using difference_type	= ptrdiff_t;

		template<typename U>
		struct rebind
		{
			using other = allocator<U>;
		};

		allocator() noexcept
		{	// construct default allocator (do nothing)
		}

		allocator(const allocator<void>&) noexcept
		{	// construct by copying (do nothing)
		}

		template<class _Other>
		allocator(const allocator<_Other>&) noexcept
		{	// construct from related allocator (do nothing)
		}

		template<class _Other>
		allocator<void>& operator=(const allocator<_Other>&)
		{	// assign from a related allocator (do nothing)
			return (*this);
		}



	};
	 


	template<typename T1, typename T2>
	inline bool operator==(const allocator<T1>& lhs, const allocator<T2>& rhs)
	{
		return true; // All allocators are considered equal, as they merely use global new/delete.
	}


	template<typename T1, typename T2>
	inline bool operator!=(const allocator<T1>& lhs, const allocator<T2>& rhs)
	{
		return false; // All allocators are considered equal, as they merely use global new/delete.
	}

}//namespace ministl

#endif //MINISTL_ALLOCATOR_HPP