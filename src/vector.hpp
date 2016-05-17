#ifndef MINISTL_VECTOR_HPP
#define MINISTL_VECTOR_HPP
#include "allocator.hpp"
#include "iterator.hpp"
#include "types.hpp"
#include "memory.hpp"
#include "utility.hpp"

namespace ministl
{
	//Reasons for containerBase(like vectorBase in this file, dequeBase for deque.hpp):
	//(According to EASTL and libcxx ) base class is for exception safety.
	//
	//
	//
	//Most STL introduce pair type like compressed_pair to store allocator and poitner
	
	//Allocator.allocate() is enough? 
	template<typename T, typename Allocator = allocator<T>>
	class vectorBase
	{
		typedef vectorBase<T, Allocator>	this_type;
	protected:
		typedef Allocator					allocator_type;
		typedef T							value_type;
		typedef size_t						size_type;
		typedef T*							pointer;
		typedef const T*					const_pointer;

	protected:
		pointer mBegin_;
		pointer mEnd_;
		pointer mCapacity_;
		allocator_type mallocator_;

		pointer vectorExpand(size_type n)
		{
			if( n > (mCapacity_ - mBegin_))
			{
				mallocator_.deallocate(mBegin_);
				mBegin_ = mallocator_.allocate(n);
			}
			return mBegin_;
		}

	public:

		vectorBase()
			: mBegin_(nullptr),
			mEnd_(nullptr),
			mCapacity_(nullptr),
			mallocator_()
		{
			
		}

		vectorBase(const allocator_type& alloc)
			: mBegin_(nullptr),
			mEnd_(nullptr),
			mCapacity_(nullptr),
			mallocator_(alloc){}

		vectorBase(size_type n, allocator_type a = allocator_type())
			:mallocator_(a)
		{
			mBegin_ = mallocator_.allocate(n);
			mEnd_ = mBegin_;
			mCapacity_ = mBegin_ + n;
		}

		~vectorBase()
		{
			mallocator_.deallocate(mBegin_);
		}

	};

	template<typename T, typename Allocator = allocator<T>>
	class vector: public vectorBase<T,Allocator>
	{
	private:
		typedef vector<T, Allocator>		this_type;
		typedef vectorBase<T, Allocator>	base_type;
		
		//TODO using base_type::vectorExpand necessary?
		using base_type::mBegin_;
		using base_type::mCapacity_;
		using base_type::mEnd_;
		using base_type::mallocator_;

		using base_type::Expand;

		//
		void _Swap(this_type &rhs)
		{
			ministl::swap(mBegin_, rhs.mBegin_);
			ministl::swap(mEnd_, rhs.mEnd_);
			ministl::swap(mCapacity_, rhs.mCapacity_);
			ministl:swap(mallocator_, rhs.mallocator_);
		}

	public:
		// types:
		typedef T							value_type;
		typedef value_type&					reference;
		typedef const value_type&			const_reference;
		typedef ptrdiff_t					difference_type;
		typedef T*							iterator;
		typedef const T*					const_iterator;

		using size_type = base_type::size_type;
		using pointer = base_type::pointer;
		using const_pointer = base_type::const_pointer;
		using allocator_type = base_type::allocator_type;

		vector() noexcept(noexcept(Allocator())) : vector(Allocator()) { }
		explicit vector(const Allocator&) noexcept;
		explicit vector(size_type n, const Allocator& = Allocator());
		vector(size_type n, const T& value, const Allocator& = Allocator());
		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const Allocator& = Allocator());
		vector(const vector& x);
		vector(vector&&) noexcept;
		vector(const vector&, const Allocator&);
		vector(vector&&, const Allocator&);
		vector(initializer_list<T>, const Allocator& = Allocator());
		
		~vector();
		const_iterator begin() const noexcept;
		iterator end() noexcept;
		const_iterator end() const noexcept;
		//reverse_iterator rbegin() noexcept;
		//const_reverse_iterator rbegin() const noexcept;
		//reverse_iterator rend() noexcept;
		//const_reverse_iterator rend() const noexcept;
		const_iterator cbegin() const noexcept;
		const_iterator cend() const noexcept;
		//const_reverse_iterator crbegin() const noexcept;
		//const_reverse_iterator crend() const noexcept;

		//capacity
		bool empty() const noexcept;
		size_type size() const noexcept;
		size_type max_size() const noexcept;
		size_type capacity() const noexcept;
		void resize(size_type sz);
		void resize(size_type sz, const T& c);
		void reserve(size_type n);
		void shrink_to_fit();


		reference operator[](size_type n);
		const_reference operator[](size_type n) const;
		const_reference at(size_type n) const;
		reference at(size_type n);
		reference front();
		const_reference front() const;
		reference back();
		const_reference back() const;
		T* data() noexcept;
		const T* data() const noexcept;


		void swap(this_type & rhs) noexcept;
	};

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(const Allocator& alloc) noexcept
		:base_type(alloc)
	{
		
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(size_type n, const Allocator& alloc)
		:base_type(n, alloc)
	{

	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(size_type n, const T& value, const Allocator& alloc)
		:base_type(n, alloc)
	{

	}

	template <typename T, typename Allocator>
	template <class InputIterator>
	vector<T, Allocator>::vector(InputIterator first, InputIterator last, const Allocator& alloc)
		:base_type(last-first, alloc)
	{

	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(const vector& x)
		:base_type(x.mCapacity_ - x.mBegin_, x.alloc)
	{
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(vector&& r) noexcept
		:base_type()
	{
		
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(const vector&, const Allocator&)
	{
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(vector&& r, const Allocator& alloc)
		:base_type(alloc)
	{
		Swap(r);
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::~vector()
	{
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::begin() const noexcept
	{
		return mBegin_;
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::end() noexcept
	{
		return mEnd_;
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::end() const noexcept
	{
		return mEnd_;
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cbegin() const noexcept
	{
		return mBegin_;
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_iterator vector<T, Allocator>::cend() const noexcept
	{
		return mEnd_;
	}

	template <typename T, typename Allocator>
	bool vector<T, Allocator>::empty() const noexcept
	{
		return (mBegin_ == mEnd_);
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::size() const noexcept
	{
		return (mBegin_ - mEnd_);
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::max_size() const noexcept
	{

	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const noexcept
	{
		return (mCapacity_ - mBegin_);
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::resize(size_type sz)
	{

	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::resize(size_type sz, const T& c)
	{

	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::reserve(size_type n)
	{

	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::shrink_to_fit()
	{
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](size_type n)
	{
		
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](size_type n) const
	{

	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type n) const
	{

	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type n)
	{

	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::front()
	{
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const
	{
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::back()
	{
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const
	{
	}

	template <typename T, typename Allocator>
	T* vector<T, Allocator>::data() noexcept
	{
	}

	template <typename T, typename Allocator>
	const T* vector<T, Allocator>::data() const noexcept
	{
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::swap(this_type& rhs) noexcept
	{

	}
} //ministl


#endif //MINISTL_VECTOR_HPP