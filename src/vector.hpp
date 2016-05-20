#ifndef MINISTL_VECTOR_HPP
#define MINISTL_VECTOR_HPP
#include "allocator.hpp"
#include "iterator.hpp"
#include "types.hpp"
#include "memory.hpp"
#include "utility.hpp"

//Layout:
//------
//vectorBase
//vector
//------

namespace ministl
{
	//Reasons for containerBase(like vectorBase in this file, dequeBase for deque.hpp):
	//(According to EASTL and libcxx )
	//exception safety.
	//
	//
	//Most STL implementations introduce pair type like compressed_pair to store allocator and poitner

	//Is Allocator.allocate() enough? 
	template <typename T, typename Allocator = allocator<T>>
	class vectorBase
	{
		typedef vectorBase<T, Allocator> this_type;
	protected:
		typedef Allocator allocator_type;
		typedef T value_type;
		typedef size_t size_type;
		typedef T* pointer;
		typedef const T* const_pointer;

	protected:
		pointer mBegin_;
		pointer mEnd_;
		pointer mCapacity_;
		allocator_type mallocator_;

		//Exception Safety?
		pointer _Reallocate_and_move(size_type n)
		{
			if (n > (mCapacity_ - mBegin_))
			{
				size_type oldsize = mEnd_ - mBegin_;
				auto TempBuf = mallocator_.allocate(n);
				ministl::uninitialized_move(mBegin_, mEnd_, TempBuf);
				mallocator_.deallocate(mBegin_);
				mBegin_ = TempBuf;
				mEnd_ = mBegin_ + oldsize;
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
			  mallocator_(alloc)
		{
		}

		vectorBase(size_type n, allocator_type a = allocator_type())
			: mallocator_(a)
		{
			mBegin_ = mallocator_.allocate(n);
			mEnd_ = mBegin_;
			mCapacity_ = mBegin_ + n;
		}

		virtual ~vectorBase()
		{
			mallocator_.deallocate(mBegin_);
		}
	};

	template <typename T, typename Allocator = allocator<T>>
	class vector: public vectorBase<T, Allocator>
	{
	private:
		typedef vector<T, Allocator> this_type;
		typedef vectorBase<T, Allocator> base_type;
	public:
		// types:
		typedef T value_type;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef ptrdiff_t difference_type;
		typedef T* iterator;
		typedef const T* const_iterator;

		using size_type = typename base_type::size_type;
		using pointer = typename base_type::pointer;
		using const_pointer = typename base_type::const_pointer;
		using allocator_type = typename base_type::allocator_type;

	private:


		//
		using base_type::mBegin_;
		using base_type::mCapacity_;
		using base_type::mEnd_;
		using base_type::mallocator_;

		//_Expand(size_type n): reallocate if n >  
		using base_type::_Reallocate_and_move;


		template <typename... Args>
		iterator _Construct(iterator position, size_type n, Args&&... args);

		//Insert for iterator input
		//should this function be added versions for different iterator catogory?
		template <typename RandomAccessIterator>
		iterator _Construct(iterator position, RandomAccessIterator begin, RandomAccessIterator end);

		//
		void _Check(size_type n);

		//
		void _Swap(this_type& rhs);

		pointer _Move(pointer begin, pointer end, pointer dest);

		inline size_type _GetNewCapacity(size_type n) const
		{
			return n ? n * 2 : 1;
		}

		inline bool _HasUnusedCapacity() const
		{
			return (mCapacity_ > mEnd_);
		}

		//_InsertAnywhere(): will double the capacity
		//_InsertAnywhereCompact():the new capacity = old capacity + n(number of inserted elements)
		template <typename... Args>
		iterator _InsertImp(iterator where, size_type n, size_type newcap, Args&&... args);

		template <typename RandomAccessIterator>
		iterator _InsertImp(iterator position, RandomAccessIterator begin, RandomAccessIterator end);

		template <typename... Args>
		iterator _InsertAnywhere(iterator where, size_type n, Args&&... args);

		template <typename... Args>
		iterator _InsertAnywhereCompact(iterator where, size_type n, Args&&... args);

	public:
		vector() noexcept: base_type(Allocator())
		{
		}

		explicit vector(const Allocator&) noexcept;
		explicit vector(size_type n, const Allocator& = Allocator());
		vector(size_type n, const T& value, const Allocator& = Allocator());
		template <class InputIterator>
		vector(InputIterator first, InputIterator last, const Allocator& = Allocator());
		vector(const this_type& x);
		vector(this_type&&) noexcept;
		vector(const this_type&, const Allocator&);
		vector(this_type&&, const Allocator&);
		//vector(initializer_list<T>, const Allocator& = Allocator());

		~vector();
		iterator begin() noexcept;
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

		template <class... Args>
		inline void emplace_back(Args&&... args);
		inline void push_back(const T& x);
		inline void push_back(T&& x);
		inline void pop_back();
		template <class... Args>
		inline iterator emplace(const_iterator position, Args&&... args);
		iterator insert(const_iterator position, const T& x);
		iterator insert(const_iterator position, T&& x);
		iterator insert(const_iterator position, size_type n, const T& x);
		template <class InputIterator>
		iterator insert(const_iterator position, InputIterator first, InputIterator last);
		//iterator insert(const_iterator position, initializer_list<T> il);
		void clear() noexcept;
		iterator erase(const_iterator position);
		iterator erase(const_iterator first, const_iterator last);
		void swap(vector&) noexcept;
		/*(allocator_traits<Allocator>::propagate_on_container_swap::value ||
			allocator_traits<Allocator>::is_always_equal::value);*/

	};


	/*Implemantation of vector
	*
	*/

	template <typename T, typename Allocator>
	template <typename ... Args>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::_Construct(iterator position, size_type n, Args&&... args)
	{
		for (; n > 0; ++position , --n)
		{
			mallocator_.construct(position, std::forward<Args>(args)...);
		}
		return position;
	}

	template <typename T, typename Allocator>
	template <typename RandomAccessIterator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::_Construct(iterator position, RandomAccessIterator begin, RandomAccessIterator end)
	{
		for (; begin != end; ++begin)
		{
			mallocator_.construct(position, *begin);
		}
		return position;
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::_Check(size_type n)
	{
		if (n >= size())
			throw std::out_of_range("ministl::vector accesses element out of range");
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::_Swap(this_type& rhs)
	{
		ministl::swap(mBegin_, rhs.mBegin_);
		ministl::swap(mEnd_, rhs.mEnd_);
		ministl::swap(mCapacity_, rhs.mCapacity_);
		ministl::swap(mallocator_, rhs.mallocator_);
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::pointer vector<T, Allocator>::_Move(pointer begin, pointer end, pointer dest)
	{
		uninitialized_move(begin, end, dest);
		return dest + (end - begin);
	}

	template <typename T, typename Allocator>
	template <typename ... Args>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::_InsertImp(iterator where, size_type n, size_type newcap, Args&&... args)
	{
		pointer newspace = mallocator_.allocate(newcap);
		pointer newwhere = _Move(mBegin_, where, newspace);
		pointer newafterinsert = _Construct(newwhere, n, std::forward<Args>(args)...);
		mEnd_ = _Move(where, mEnd_, newafterinsert);
		mBegin_ = newspace;
		mCapacity_ = mBegin_ + newcap;
		return newwhere + 1;
	}

	template <typename T, typename Allocator>
	template <typename RandomAccessIterator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::_InsertImp(iterator position, RandomAccessIterator begin, RandomAccessIterator end)
	{
		size_type newcap = size() + begin - end;
		pointer newspace = mallocator_.allocate(newcap);
		pointer newwhere = _Move(mBegin_, position, newspace);
		pointer newafterinsert = _Construct(newwhere, begin, end);
		mEnd_ = _Move(position, mEnd_, newafterinsert);
		mBegin_ = newspace;
		mCapacity_ = mBegin_ + newcap;
		return newwhere + 1;
	}

	template <typename T, typename Allocator>
	template <typename ... Args>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::_InsertAnywhere(iterator where, size_type n, Args&&... args)
	{
		if (n != 0)
		{
			//newcap =  2*(size() + n). 
			size_type newcap = _GetNewCapacity(n + size());
			return _InsertImp(where, n, newcap, std::forward<Args>(args)...);
		}
		return where;
	}

	template <typename T, typename Allocator>
	template <typename ... Args>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::_InsertAnywhereCompact(iterator where, size_type n, Args&&... args)
	{
		if (n != 0)
		{
			//newcap =  2*(size() + n). 
			size_type newcap = size() + n;
			return _InsertImp(where, n, newcap, std::forward<Args>(args)...);
		}
		return where;
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(const Allocator& alloc) noexcept
		: base_type(alloc)
	{
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(size_type n, const Allocator& alloc)
		: base_type(n, alloc)
	{
		mEnd_ = mCapacity_;
		_Construct(mBegin_, n);
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(size_type n, const T& value, const Allocator& alloc)
		: base_type(n, alloc)
	{
		_Construct(mBegin_, n, value);
		mEnd_ = mBegin_ + n;
	}

	//! vector<int>(2,3)will call this constructor althought it means to call vector<int>(size_t, int&)
	// TODO: add the following template argument.
	//	template<class _Iter,
	//	class = typename enable_if<_Is_iterator<_Iter>::value,
	//		void>::type>
	template <typename T, typename Allocator>
	template <class InputIterator>
	vector<T, Allocator>::vector(InputIterator first, InputIterator last, const Allocator& alloc = Allocator())
		: base_type(last - first, alloc)
	{
		ministl::uninitialized_copy(first, last, mBegin_);
		mEnd_ = mBegin_ + (last - first);
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(const this_type& x)
		: base_type(x.mCapacity_ - x.mBegin_, x.mallocator_)
	{
		ministl::uninitialized_copy(x.mBegin_, x.mEnd_, mBegin_);
		mEnd_ = mBegin_ + x.size();
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(this_type&& r) noexcept
		: base_type()
	{
		_Swap(r);
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(const this_type& that, const Allocator& alloc)
		: base_type(that.size(), alloc)
	{
		ministl::uninitialized_copy(that.mBegin_, that.mEnd_, mBegin_);
		mEnd_ = mBegin_ + that.size();
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::vector(this_type&& r, const Allocator& alloc)
		: base_type()
	{
		_Swap(r);
	}

	template <typename T, typename Allocator>
	vector<T, Allocator>::~vector()
	{
		destroy(mBegin_, mEnd_);
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::begin() noexcept
	{
		return mBegin_;
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
		return (mEnd_ - mBegin_);
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::max_size() const noexcept
	{
		return mallocator_.max_size();
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::size_type vector<T, Allocator>::capacity() const noexcept
	{
		return (mCapacity_ - mBegin_);
	}

	//C++11:
	//if sz > size():default insert(?)
	//if sz < size():shrink
	template <typename T, typename Allocator>
	void vector<T, Allocator>::resize(size_type sz)
	{
		if(sz >= size())
		{
			size_type oldsize = size();
			_Reallocate_and_move(sz);
			_Construct(mBegin_ + oldsize, sz - oldsize);
			mEnd_ = mCapacity_;
		}
		else
		{
			destroy(mBegin_ + sz, mEnd_);
			mEnd_ = mBegin_ + sz;
		}
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::resize(size_type sz, const T& c)
	{
		if(sz > size())
		{
			size_type oldsize = size();
			_Reallocate_and_move(sz);
			_Construct(mBegin_ + oldsize, sz - oldsize, c);
			mEnd_ = mCapacity_;
		}
		else
		{
			destroy(mBegin_ + sz, mEnd_);
			mEnd_ = mBegin_ + sz;
		}
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::reserve(size_type n)
	{
		_Reallocate_and_move(n);
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::shrink_to_fit()
	{
		if (_HasUnusedCapacity())
		{
			this_type(*this).swap(*this);
		}
	}

	//
	template <typename T, typename Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::operator[](size_type n)
	{
		return *(mBegin_ + n);
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::operator[](size_type n) const
	{
		return *(mBegin_ + n);
	}

	//difference between at() and operator[]:
	//the latter requires bound checking(check()).
	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::at(size_type n) const
	{
		_Check(n);
		return *this[n];
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::at(size_type n)
	{
		_Check(n);
		return *this[n];
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::front()
	{
		return *mBegin_;
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::front() const
	{
		return *mBegin_;
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::reference vector<T, Allocator>::back()
	{
		return *(mEnd_ - 1);
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::const_reference vector<T, Allocator>::back() const
	{
		return *(mEnd_ - 1);
	}

	template <typename T, typename Allocator>
	T* vector<T, Allocator>::data() noexcept
	{
		return mBegin_;
	}

	template <typename T, typename Allocator>
	const T* vector<T, Allocator>::data() const noexcept
	{
		return mBegin_;
	}

	template <typename T, typename Allocator>
	template <class ... Args>
	inline void vector<T, Allocator>::emplace_back(Args&&... args)
	{
		if (_HasUnusedCapacity())
		{
			::new(static_cast<void*>(mEnd_)) value_type(std::forward<Args>(args)...);
			++mEnd_;
		}
		else
		{
			_InsertAnywhere(mEnd_, 1, args);
		}
	}

	template <typename T, typename Allocator>
	inline void vector<T, Allocator>::push_back(const T& x)
	{
		if (_HasUnusedCapacity())
		{
			::new(static_cast<void*>(mEnd_)) value_type(x);
			++mEnd_;
		}
		else
		{
			_InsertAnywhere(mEnd_, 1, x);
		}
	}

	template <typename T, typename Allocator>
	inline void vector<T, Allocator>::push_back(T&& x)
	{
		if (_HasUnusedCapacity())
		{
			::new(static_cast<void*>(mEnd_)) value_type(ministl::move(x));
			++mEnd_;
		}
		else
		{
			_InsertAnywhere(mEnd_, 1, ministl::move(x));
		}
	}

	template <typename T, typename Allocator>
	inline void vector<T, Allocator>::pop_back()
	{
		mallocator_.destruct(mEnd_);
		--mEnd_;
	}

	template <typename T, typename Allocator>
	template <class ... Args>
	inline typename vector<T, Allocator>::iterator vector<T, Allocator>::emplace(const_iterator position, Args&&... args)
	{
		return _InsertAnywhereCompact(position, 1, std::forward<Args>(args)...);
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position, const T& x)
	{
		return _InsertAnywhereCompact(position, 1, x);
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position, T&& x)
	{
		return _InsertAnywhereCompact(position, 1, ministl::move(x));
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position, size_type n, const T& x)
	{
		return _InsertAnywhereCompact(position, n, x);
	}

	template <typename T, typename Allocator>
	template <class InputIterator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::insert(const_iterator position, InputIterator first, InputIterator last)
	{
		return _InsertImp(position, first, last);
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::clear() noexcept
	{
		destroy(mBegin_, mEnd_);
		mEnd_ = mBegin_;
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator position)
	{
		position->~value_type();
		pointer position_noconst = const_cast<pointer>(position);
		pointer currentposition = const_cast<pointer>(position) + 1;
		uninitialized_move(currentposition, mEnd_, position_noconst);
		--mEnd_;
		return position_noconst;
	}

	template <typename T, typename Allocator>
	typename vector<T, Allocator>::iterator vector<T, Allocator>::erase(const_iterator first, const_iterator last)
	{
		if ((last - first) == size())
		{
			clear();
			return nullptr;
		}
		else {
			destroy(first, last);
			iterator last_noconst = const_cast<iterator>(last);
			iterator first_noconst = const_cast<iterator>(first);
			mEnd_ = uninitialized_move(last_noconst, mEnd_, first_noconst);
			return last_noconst;
		}
	}

	template <typename T, typename Allocator>
	void vector<T, Allocator>::swap(this_type& rhs) noexcept
	{
		_Swap(rhs);
	}
} //ministl


#endif //MINISTL_VECTOR_HPP
