//shared_ptr
//

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef MINISTL_SHARED_PTR_HPP
#define MINISTL_SHARED_PTR_HPP

#include "allocator.hpp"
#include "unique_ptr.hpp"
#include "utility.hpp"

#include <cstddef>

//Notes from LunAnalects, 2016/6/19
//Why no allocator or deleter in shared_ptr template:

//Unlike unique_ptr, shared_ptr doesn't need allocator or deleter as template argument. They are just simply passed as parameter in constructor.
//Internally, reference count member class in shared pointer is responsible to store the allocator and deleter. so reference count class is a 3-arguments
//template. So we need use an reference_count_base * member class in smart pointer to store the template reference_count<Ptr, Deleter, Alloc> which is 
//inherited from reference_count_base, becuase we cannot directly store an template member class in our smart pointer without 
//deleter and allocator template argument
//


//Notes from LunAnalects, 2016/6/19
//Allocator:

//Allocator in smart pointer is to allocate the memory for smart pointer member, that is, reference count member class.
//So, it is impossible to pass an template allocator instantiated by reference because reference need be instantiated by allocator too.
//Solution in EASTL: they pass an nontemplate allocator which allocate memory like malloc();
//Solution in VC:	
//

namespace ministl
{
	template<class T> class weak_ptr;

	namespace internal
	{
		//Difference between internal::type_erasure_deleter and default_delete in unique_ptr.hpp?
		template<typename T>
		struct type_erased_deleter
		{
			void operator()(void* p) const
			{
				delete static_cast<T*>(p);
			}
		};

		template<typename T>
		struct type_erased_deleter<T[]>
		{
			void operator()(void* p) const
			{
				delete[] static_cast<T*>(p);
			}
		};

		class type_erased_allocator
		{
			
		};
	}

	//template<typename T>
	//class pointer_trait

	//template<typename T>
	//class pointer_trait<T*>
	//{
	//	
	//};

	class reference_count_base
	{
	public:
		reference_count_base() :sharedCount_(1), weakCount_(1) {}

		virtual ~reference_count_base() {};

		virtual void release() = 0;

		int32_t use_count() const noexcept
		{
			return sharedCount_;
		}

		void addref() noexcept
		{
			increShared();
			increWeak();
		}

	protected:
		int32_t increShared() noexcept
		{
			return ++sharedCount_;
		}

		int32_t decreShared() noexcept
		{
			return --sharedCount_;
		}

		int32_t increWeak() noexcept
		{
			return ++weakCount_;
		}

		int32_t decreWeak() noexcept
		{
			return --weakCount_;
		}

		int32_t getShared() const noexcept
		{
			return sharedCount_;
		}

		int32_t getWeak() const noexcept
		{
			return weakCount_;
		}

	private:
		int32_t sharedCount_;
		int32_t weakCount_;
	};

	template<typename T, typename Deleter, typename  Alloc>
	class reference_count_: public reference_count_base
	{
	public:

		using this_type = reference_count_<T, Deleter, Alloc>;

		using allocator_type = typename internal::__allocator_traits_rebind<Alloc, this_type>::type;

		//Alloc and this_type: is these two template mutually dependent?
		reference_count_(T ptr, Deleter deleter, Alloc alloc)
			:elePtr_(ptr), deleter_(deleter), allocator_(){}

		virtual ~reference_count_() override final
		{
			deleter_(elePtr_);
		} 

		virtual void release() override final
		{
			if (decreShared() > 0)
			{
				decreWeak();
			}
			else
			{
				freeThisInstance();
			}
		}
		
		//Make reference_count_ class noncopyable. 
		reference_count_(const this_type &) = delete;
		this_type & reference_count(const this_type &) = delete;

	private:
		T elePtr_;
		Deleter deleter_;
		allocator_type allocator_;

		void freeThisInstance()
		{
			deleter_(elePtr_);
		}
	};

	template<typename T>
	class shared_ptr
	{
	public:
		typedef T element_type;

		// 20.10.2.2.1, constructors:
		constexpr shared_ptr() noexcept;
		template<class Y> explicit shared_ptr(Y* p);
		template<class Y, class D> shared_ptr(Y* p, D d);
		template<class Y, class D, class A> shared_ptr(Y* p, D d, A a);
		template <class D> shared_ptr(nullptr_t p, D d);
		template <class D, class A> shared_ptr(nullptr_t p, D d, A a);
		template<class Y> shared_ptr(const shared_ptr<Y>& r, T* p) noexcept;
		shared_ptr(const shared_ptr& r) noexcept;
		template<class Y> shared_ptr(const shared_ptr<Y>& r) noexcept;
		shared_ptr(shared_ptr&& r) noexcept;
		template<class Y> shared_ptr(shared_ptr<Y>&& r) noexcept;
		template<class Y> explicit shared_ptr(const weak_ptr<Y>& r);
		template <class Y, class D> shared_ptr(unique_ptr<Y, D>&& r);
		constexpr shared_ptr(nullptr_t) noexcept : shared_ptr() { }

		// 20.10.2.2.2, destructor:
		~shared_ptr();

		// 20.10.2.2.3, assignment:
		shared_ptr& operator=(const shared_ptr& r) noexcept;
		template<class Y> shared_ptr& operator=(const shared_ptr<Y>& r) noexcept;
		shared_ptr& operator=(shared_ptr&& r) noexcept;
		template<class Y> shared_ptr& operator=(shared_ptr<Y>&& r) noexcept;
		template <class Y, class D> shared_ptr& operator=(unique_ptr<Y, D>&& r);

		// 20.10.2.2.4, modifiers:
		void swap(shared_ptr& r) noexcept;
		void reset() noexcept;
		template<class Y> void reset(Y* p);
		template<class Y, class D> void reset(Y* p, D d);
		template<class Y, class D, class A> void reset(Y* p, D d, A a);

		// 20.10.2.2.5, observers:
		T* get() const noexcept;
		T& operator*() const noexcept;
		T* operator->() const noexcept;
		long use_count() const noexcept;
		bool unique() const noexcept;
		explicit operator bool() const noexcept;
		template<class U> bool owner_before(shared_ptr<U> const& b) const;
		template<class U> bool owner_before(weak_ptr<U> const& b) const;

	private:
		T  *elementPtr_;
		reference_count_base *refCountPtr_;

		//This function will throw std::bad_alloc if allocator failed to allocate memory for reference count object
		//TODO 
		template<typename Ptr, typename Deleter, typename Alloc = ministl::allocator<char>>
		void _AllocRefCountPtr(Ptr pValue, Deleter deleter, Alloc allocator = Alloc())
		{
			try
			{
				refCountPtr_ = allocator.allocate(1);
				if (!refCountPtr_)
					throw std::bad_alloc();
				allocator.construct(refCountPtr_, pValue, ministl::move(deleter), ministl::move(allocator));
				//do_enable_shared_from_this?
			}
			catch(...)
			{
				deleter(pValue);

				//If allocator.construct() throws exception, we need then deallocate the memory, 
				//but could the construct() throw exception?
				//And, if construct() use move() to move construct the allocator, we maynot call allocator again.(maybe it depends on implementation)

				//if (refCountPtr_)
				//	allocator.deallocate(refCountPtr_);
				throw;
			}
		}
		
	};

	template <typename T>
	constexpr shared_ptr<T>::shared_ptr() noexcept
		:elementPtr_(nullptr),refCountPtr_(nullptr)
	{
		//Don't throw exception.
	}

	template <typename T>
	template <typename Y>
	shared_ptr<T>::shared_ptr(Y* p)
		: elementPtr_(p),
		refCountPtr_(nullptr) 
	{
		_AllocRefCountPtr(p, default_delete<T>());
	}

	template <typename T>
	template <class Y, class D>
	shared_ptr<T>::shared_ptr(Y* p, D d)
		: elementPtr_(p), 
		refCountPtr_(nullptr)
	{
		_AllocRefCountPtr(p, d);
	}

	template <typename T>
	template <class Y, class D, class A>
	shared_ptr<T>::shared_ptr(Y* p, D d, A a)
		:elementPtr_(P), 
		refCountPtr_(nullptr)
	{
		_AllocRefCountPtr(p, d, a);
	}

	template <typename T>
	template <class D>
	shared_ptr<T>::shared_ptr(nullptr_t p, D d)
		:elementPtr_(p),
		refCountPtr_(nullptr)
	{
		_AllocRefCountPtr(p, d);
	}

	template <typename T>
	template <class D, class A>
	shared_ptr<T>::shared_ptr(nullptr_t p, D d, A a)
		:elementPtr_(p),
		refCountPtr_(nullptr)
	{
		_AllocRefCountPtr(p, d, a);
	}

	template <typename T>
	template <class Y>
	shared_ptr<T>::shared_ptr(const shared_ptr<Y>& r, T* p) noexcept
		:elementPtr_(p),
		refCountPtr_(r.refCountPtr_)
	{
		if(r.refCountPtr_)
			r.refCountPtr_->addref();
	}

	template <typename T>
	shared_ptr<T>::shared_ptr(const shared_ptr& r) noexcept
		:elementPtr_(r.elementPtr_), refCountPtr_(r.refCountPtr_)
	{
		if (r.refCountPtr_)
			r.refCountPtr_->addref();
	}

	template <typename T>
	template <class Y>
	shared_ptr<T>::shared_ptr(const shared_ptr<Y>& r) noexcept
		:elementPtr_(r.elementPtr_), refCountPtr_(r.refCountPtr_)
	{
		if (r.refCountPtr_)
			r.refCountPtr_->addref();
	}

	template <typename T>
	shared_ptr<T>::shared_ptr(shared_ptr&& r) noexcept
		:elementPtr_(r.elementPtr_), refCountPtr_(r.refCountPtr_)
	{
		r.elementPtr_ = nullptr;
		r.refCountPtr_ = nullptr;
	}

	template <typename T>
	template <class Y>
	shared_ptr<T>::shared_ptr(shared_ptr<Y>&& r) noexcept
		:elementPtr_(r.elementPtr_), refCountPtr_(r.refCountPtr_)
	{
		r.elementPtr_ = nullptr;
		r.refCountPtr_ = nullptr;
	}

	template <typename T>
	template <class Y, class D>
	shared_ptr<T>::shared_ptr(unique_ptr<Y, D>&& r)
		:elementPtr_(r.release()), refCountPtr_(nullptr)
	{
		_AllocRefCountPtr(elementPtr_, r.get_deleter());
	}

	template <typename T>
	shared_ptr<T>::~shared_ptr()
	{
		refCountPtr_->release();
	}

	template <typename T>
	shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr& r) noexcept
	{

	}

	template <typename T>
	template <class Y>
	shared_ptr<T>& shared_ptr<T>::operator=(const shared_ptr<Y>& r) noexcept
	{
	}

	template <typename T>
	shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr&& r) noexcept
	{
	}

	template <typename T>
	template <class Y>
	shared_ptr<T>& shared_ptr<T>::operator=(shared_ptr<Y>&& r) noexcept
	{
	}

	template <typename T>
	template <class Y, class D>
	shared_ptr<T>& shared_ptr<T>::operator=(unique_ptr<Y, D>&& r)
	{
	}

	template <typename T>
	void shared_ptr<T>::swap(shared_ptr& r) noexcept
	{
	}

	template <typename T>
	void shared_ptr<T>::reset() noexcept
	{
	}

	template <typename T>
	template <class Y>
	void shared_ptr<T>::reset(Y* p)
	{
	}

	template <typename T>
	template <class Y, class D>
	void shared_ptr<T>::reset(Y* p, D d)
	{
	}

	template <typename T>
	template <class Y, class D, class A>
	void shared_ptr<T>::reset(Y* p, D d, A a)
	{
	}

	template <typename T>
	T* shared_ptr<T>::get() const noexcept
	{
		return elementPtr_;
	}

	template <typename T>
	T& shared_ptr<T>::operator*() const noexcept
	{
		return *elementPtr_;
	}

	template <typename T>
	T* shared_ptr<T>::operator->() const noexcept
	{

	}

	template <typename T>
	long shared_ptr<T>::use_count() const noexcept
	{
		return refCountPtr_ ? refCountPtr_->use_count() : 0;
	}

	template <typename T>
	bool shared_ptr<T>::unique() const noexcept
	{
		return (refCountPtr_ && (refCountPtr_->use_count() == 1));
	}

	template <typename T>
	shared_ptr<T>::operator bool() const noexcept
	{
		return elementPtr_ == NULL;
	}

	template <typename T>
	template <class U>
	bool shared_ptr<T>::owner_before(shared_ptr<U> const& b) const
	{
	}

	template <typename T>
	template <class U>
	bool shared_ptr<T>::owner_before(weak_ptr<U> const& b) const
	{
	}

	// 20.10.2.2.6, shared_ptr creation
	template<class T, class... Args> shared_ptr<T> make_shared(Args&&... args);
	template<class T, class A, class... Args>
	shared_ptr<T> allocate_shared(const A& a, Args&&... args);

	// 20.10.2.2.7, shared_ptr comparisons:
	template<class T, class U>
	bool operator==(const shared_ptr<T>& a, const shared_ptr<U>& b) noexcept;
	template<class T, class U>
	bool operator!=(const shared_ptr<T>& a, const shared_ptr<U>& b) noexcept;
	template<class T, class U>
	bool operator<(const shared_ptr<T>& a, const shared_ptr<U>& b) noexcept;
	template<class T, class U>
	bool operator>(const shared_ptr<T>& a, const shared_ptr<U>& b) noexcept;
	template<class T, class U>
	bool operator<=(const shared_ptr<T>& a, const shared_ptr<U>& b) noexcept;
	template<class T, class U>
	bool operator>=(const shared_ptr<T>& a, const shared_ptr<U>& b) noexcept;
	template <class T>
	bool operator==(const shared_ptr<T>& a, nullptr_t) noexcept;
	template <class T>
	bool operator==(nullptr_t, const shared_ptr<T>& b) noexcept;
	template <class T>
	bool operator!=(const shared_ptr<T>& a, nullptr_t) noexcept;
	template <class T>
	bool operator!=(nullptr_t, const shared_ptr<T>& b) noexcept;
	template <class T>
	bool operator<(const shared_ptr<T>& a, nullptr_t) noexcept;
	template <class T>
	bool operator<(nullptr_t, const shared_ptr<T>& b) noexcept;
	template <class T>
	bool operator<=(const shared_ptr<T>& a, nullptr_t) noexcept;
	template <class T>
	bool operator<=(nullptr_t, const shared_ptr<T>& b) noexcept;
	template <class T>
	bool operator>(const shared_ptr<T>& a, nullptr_t) noexcept;
	template <class T>
	bool operator>(nullptr_t, const shared_ptr<T>& b) noexcept;
	template <class T>
	bool operator>=(const shared_ptr<T>& a, nullptr_t) noexcept;
	template <class T>
	bool operator>=(nullptr_t, const shared_ptr<T>& b) noexcept;

	// 20.10.2.2.8, shared_ptr specialized algorithms:
	template<class T> void swap(shared_ptr<T>& a, shared_ptr<T>& b) noexcept;

	// 20.10.2.2.9, shared_ptr casts:
	template<class T, class U>
	shared_ptr<T> static_pointer_cast(const shared_ptr<U>& r) noexcept;
	template<class T, class U>
	shared_ptr<T> dynamic_pointer_cast(const shared_ptr<U>& r) noexcept;
	template<class T, class U>
	shared_ptr<T> const_pointer_cast(const shared_ptr<U>& r) noexcept;

	// 20.10.2.2.10, shared_ptr get_deleter:
	template<class D, class T> D* get_deleter(const shared_ptr<T>& p) noexcept;


	template<class T> class weak_ptr {
	public:
		typedef T element_type;

		// 20.10.2.3.1, constructors
		constexpr weak_ptr() noexcept;
		template<class Y> weak_ptr(shared_ptr<Y> const& r) noexcept;
		weak_ptr(weak_ptr const& r) noexcept;
		template<class Y> weak_ptr(weak_ptr<Y> const& r) noexcept;
		weak_ptr(weak_ptr&& r) noexcept;
		template<class Y> weak_ptr(weak_ptr<Y>&& r) noexcept;

		// 20.10.2.3.2, destructor
		~weak_ptr();

		// 20.10.2.3.3, assignment
		weak_ptr& operator=(weak_ptr const& r) noexcept;
		template<class Y> weak_ptr& operator=(weak_ptr<Y> const& r) noexcept;  
		template<class Y> weak_ptr& operator=(shared_ptr<Y> const& r) noexcept;
		weak_ptr& operator=(weak_ptr&& r) noexcept;
		template<class Y> weak_ptr& operator=(weak_ptr<Y>&& r) noexcept;

		// 20.10.2.3.4, modifiers
		void swap(weak_ptr& r) noexcept;
		void reset() noexcept;

		// 20.10.2.3.5, observers
		long use_count() const noexcept;
		bool expired() const noexcept;
		shared_ptr<T> lock() const noexcept;
		template<class U> bool owner_before(shared_ptr<U> const& b) const;
		template<class U> bool owner_before(weak_ptr<U> const& b) const;

	private:

	};

	// 20.10.2.3.6, specialized algorithms
	template<class T> void swap(weak_ptr<T>& a, weak_ptr<T>& b) noexcept;

	template <class T>
	constexpr weak_ptr<T>::weak_ptr() noexcept
	{
	}

	template <class T>
	weak_ptr<T>::weak_ptr(weak_ptr const& r) noexcept
	{
	}

	template <class T>
	template <class Y>
	weak_ptr<T>::weak_ptr(weak_ptr<Y> const& r) noexcept
	{
	}

	template <class T>
	weak_ptr<T>::weak_ptr(weak_ptr&& r) noexcept
	{
	}

	template <class T>
	template <class Y>
	weak_ptr<T>::weak_ptr(weak_ptr<Y>&& r) noexcept
	{
	}

	template <class T>
	weak_ptr<T>::~weak_ptr()
	{
	}

	template <class T>
	weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr const& r) noexcept
	{
	}

	template <class T>
	template <class Y>
	weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr<Y> const& r) noexcept
	{
	}

	template <class T>
	weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr&& r) noexcept
	{
	}

	template <class T>
	template <class Y>
	weak_ptr<T>& weak_ptr<T>::operator=(weak_ptr<Y>&& r) noexcept
	{
	}

	template <class T>
	void weak_ptr<T>::swap(weak_ptr& r) noexcept
	{
	}

	template <class T>
	void weak_ptr<T>::reset() noexcept
	{
	}

	template <class T>
	long weak_ptr<T>::use_count() const noexcept
	{
	}

	template <class T>
	bool weak_ptr<T>::expired() const noexcept
	{
	}

	template <class T>
	template <class U>
	bool weak_ptr<T>::owner_before(weak_ptr<U> const& b) const
	{
	}

	template <class T>
	void swap(weak_ptr<T>& a, weak_ptr<T>& b) noexcept
	{
	}
	
	//TODO: enable_shared_from_this need some tricks, not completed yet.
	//see EASTL share_ptr.h LINE380:do_enable_shared_from_this
	template<typename T>
	class enable_shared_from_this
	{
	public:
		virtual ~enable_shared_from_this() {}

		shared_ptr<T> shared_from_this()
		{
			return shared_ptr<T>(weak_this);
		}

		shared_ptr<const T> shared_from_this() const
		{
			return shared_ptr<T>(weak_this);
		}
		weak_ptr<T> weak_from_this() {
			return weak_ptr<T>(weak_this);
		}

		weak_ptr<const T> weak_from_this() const {
			return weak_ptr<T>(weak_this);
		}

	protected:

	private:
		weak_ptr<T> weak_this;
	};

} //namespace nimistl

#endif //MINISTL_SHARED_PTR_HPP