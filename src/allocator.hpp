#ifndef MINISTL_ALLOCATOR_HPP
#define MINISTL_ALLOCATOR_HPP
#include "types.hpp"
#include "stdlib.h"
namespace ministl
{
	template <typename T>
	class SimpleAllocator
	{
		using value_type = T;

		SimpleAllocator();
		template <typename U> SimpleAllocator(const SimpleAllocator<U>& other);

		T* allocate(size_t n);


		void deallocate(T *p, size_t n);

	};

	template<typename T, typename U>
	bool operator==(const SimpleAllocator<T>&, const SimpleAllocator<U>&);

	template<typename T, typename U>
	bool operator!=(const SimpleAllocator<T>&, const SimpleAllocator<U>&);


	class small_allocator
	{
	private:
		enum ALIGN_CONFIG
		{
			ALIGN = 8,
			MAX_BYTES = 128,
			NFREELISTS = ALIGN / MAX_BYTES
		};
	
	private:
		static size_t round_up (size_t bytes)
		{
			return ((bytes + (ALIGN - 1)) & ~(ALIGN - 1));
		}

		union obj
		{
			union obj *free_list_link;
			char client_Data[1];
		};

	private:
		static obj * free_list[NFREELISTS];

		static size_t freelist_index(size_t bytes)
		{
			return (((bytes)+ALIGN - 1) / ALIGN - 1);
		}

		static void *refill(size_t n);

		static char *chunk_alloc(size_t size, int &nobjs);

		static char *start_free;

		static char *end_free;
		static size_t heap_size;

	public:
		static void *allocate(size_t n);
		static void dealocate(void *p, size_t n);
		static void *reallocate(void *p, size_t old_sz, size_t new_sz);
	};

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
		allocator(const this_type &) = delete;
		this_type& operator=(const this_type &) = delete;
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
		
		pointer allocate(size_type n, allocator<void>::const_pointer hint = nullptr)
		{
			size_type userSize = n * sizeof(value_type);
			pointer allocated_ptr = nullptr;
			if(userSize >0 && userSize < static_cast<size_t>(-1) )
				allocated_ptr = static_cast<pointer>(malloc( n * sizeof(value_type) ));
			return allocated_ptr;
		}

		void deallocate(pointer p)
		{
			free(p);
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
		template <typename U, typename... Args>
		void construct(U* p, Args&&... args)
		{
			//TODO a forward() to wrap args
			::new (static_cast<void *>(p)) U(forward<Args>(args)...);

		}
		
		template<typename U>
		void destroy(U* p)
		{
			p->~U();
		}

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
	 

}//namespace ministl

#endif //MINISTL_ALLOCATOR_HPP