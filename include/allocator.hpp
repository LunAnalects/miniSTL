#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef MINISTL_ALLOCATOR_HPP
#define MINISTL_ALLOCATOR_HPP

#include "types.hpp"
#include <stdlib.h>
#include <type_traits>

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

		template <typename T>
		struct rebind { typedef allocator<T> other; };
		template <typename T>
		using rebind_t = allocator<T>;

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

	namespace internal {
		//From libcxx
		//
		template <class _Tp, class _Up>
		struct __has_rebind
		{
		private:
			struct __two { char __lx; char __lxx; };
			template <class _Xp> static __two __test(...);
			template <class _Xp> static char __test(typename _Xp::template rebind<_Up>* = 0);
		public:
			static const bool value = sizeof(__test<_Tp>(0)) == 1;
		};

		template <class _Tp, class _Up, bool = __has_rebind<_Tp, _Up>::value>
		struct __has_rebind_other
		{
		private:
			struct __two { char __lx; char __lxx; };
			template <class _Xp> static __two __test(...);
			template <class _Xp> static char __test(typename _Xp::template rebind<_Up>::other* = 0);
		public:
			static const bool value = sizeof(__test<_Tp>(0)) == 1;
		};

		template <class _Tp, class _Up>
		struct __has_rebind_other<_Tp, _Up, false>
		{
			static const bool value = false;
		};

		template <class _Tp, class _Up, bool = __has_rebind_other<_Tp, _Up>::value>
		struct __allocator_traits_rebind
		{
			typedef typename _Tp::template rebind<_Up>::other type;
		};

		template <template <class, class...> class _Alloc, class _Tp, class ..._Args, class _Up>
		struct __allocator_traits_rebind<_Alloc<_Tp, _Args...>, _Up, true>
		{
			typedef typename _Alloc<_Tp, _Args...>::template rebind<_Up>::other type;
		};

		template <template <class, class...> class _Alloc, class _Tp, class ..._Args, class _Up>
		struct __allocator_traits_rebind<_Alloc<_Tp, _Args...>, _Up, false>
		{
			typedef _Alloc<_Up, _Args...> type;
		};
	}

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
	 
	template<typename Alloc>
	struct allocator_traits {
		using allocator_type = Alloc;
		using value_type =typename Alloc::value_type;
		//using pointer = ;
		//using const_pointer = ;
		//using void_pointer = ;
		//using difference_type = ;
		//using size_type =
		//using propagate_on_container_copy_assignment =
		//using propagate_on_container_move_assignment =
		//using propagate_on_contaner_swap
		//using is_always_equal = ;

		template<typename Other>
		using rebind_alloc = internal::__allocator_traits_rebind<Alloc, Other>;

		//static allocate(){}
		//static deallocate(){}
		//static construct(){}
		//static destroy(){}
		//static max_size(){}
		//static select_on_container_copy_construction(){}

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