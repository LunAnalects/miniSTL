#ifndef MINISTL_SHARED_PTR_HPP
#define MINISTL_SHARED_PTR_HPP

#include "weak_ptr.hpp"
#include "unique_ptr.hpp"

#include <cstddef>

namespace ministl
{
	namespace internal
	{
		//Difference between internal::type_erasure_deleter and default_delete in unique_ptr.hpp?
		template<typename T>
		struct type_erasure_deleter
		{
			void operator()(void* p) const
			{
				delete static_cast<T*>(p);
			}
		};

	}
	class reference_count_base
	{
	public:
		reference_count_base() :sharedCount_(1), weakCount_(1) {}

		virtual ~reference_count_base(){}

		int32_t use_count() const noexcept
		{
			return sharedCount_;
		}

		void addref() noexcept
		{
			increShared();
			increWeak();
		}

		virtual void release()
		{
			if (decreShared() > 0)
			{
				decreWeak();
			}
			else
			{
			}
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

	//It's the final class, don't derive from it
	template<typename T, typename Deleter, typename  Alloc>
	class reference_count_: public reference_count_base
	{
	public:
		reference_count_(T ptr, Deleter deleter, Alloc allocator)
			:elePtr(ptr), deleter_(deleter), allocator_(allocator){}

		virtual ~reference_count_() override final
		{
			//Calling a virtual function in destructor is arguable(Effective C++, Item 9), 
			//but here I call it in the final derived destructor, which is obviously harmless
			release();
		} 

		virtual void release() override
		{
			if (decreShared() > 0)
			{
				decreWeak();
			}
			else
			{
				deleter_(elePtr);
			}
		}

	private:
		T* elePtr;
		Deleter deleter_;
		Alloc allocator_;


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
		T* elementPtr_;
		reference_count_base* refCountPtr_;

		template<typename Deleter, typename Alloc>
		reference_count_base* _allocRefCountPtr(Deleter deleter = Deleter(), Alloc allocator = Alloc())
		{
			return (new reference_count_<T, Deleter, Alloc>(elementPtr_, deleter, allocator));
		}
		
	};

	template <typename T>
	constexpr shared_ptr<T>::shared_ptr() noexcept
		:elementPtr_(NULL),refCountPtr_(NULL)
	{
	}

	template <typename T>
	template <class Y>
	shared_ptr<T>::shared_ptr(Y* p)
		: elementPtr_(p),
		refCountPtr_(_allocRefCountPtr())
	{
	}

	template <typename T>
	template <class Y, class D>
	shared_ptr<T>::shared_ptr(Y* p, D d)
		: elementPtr_(p), 
		refCountPtr_(_allocRefCountPtr(d))
	{
	}

	template <typename T>
	template <class Y, class D, class A>
	shared_ptr<T>::shared_ptr(Y* p, D d, A a)
		:elementPtr_(P), 
		refCountPtr_(d, a)
	{

	}

	template <typename T>
	template <class D>
	shared_ptr<T>::shared_ptr(nullptr_t p, D d)
		:elementPtr_(p),
		refCountPtr_(d)
	{
	}

	template <typename T>
	template <class D, class A>
	shared_ptr<T>::shared_ptr(nullptr_t p, D d, A a)
		:elementPtr_(p),
		refCountPtr_(d, a)
	{
	}

	template <typename T>
	template <class Y>
	shared_ptr<T>::shared_ptr(const shared_ptr<Y>& r, T* p) noexcept
		:elementPtr_(p),
		refCountPtr_(NULL)
	{
		r.refCountPtr_->addref();
		refCountPtr_ = r.refCountPtr_;
	}

	template <typename T>
	shared_ptr<T>::shared_ptr(const shared_ptr& r) noexcept
		:elementPtr_(r.elementPtr_), refCountPtr_(NULL)
	{
		r.refCountPtr_->addref();
		refCountPtr_ = 
	}

	template <typename T>
	template <class Y>
	shared_ptr<T>::shared_ptr(const shared_ptr<Y>& r) noexcept
	{
	}

	template <typename T>
	shared_ptr<T>::shared_ptr(shared_ptr&& r) noexcept
	{
	}

	template <typename T>
	template <class Y>
	shared_ptr<T>::shared_ptr(shared_ptr<Y>&& r) noexcept
	{
	}

	template <typename T>
	template <class Y, class D>
	shared_ptr<T>::shared_ptr(unique_ptr<Y, D>&& r)
	{
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
	}

	template <typename T>
	T& shared_ptr<T>::operator*() const noexcept
	{
	}

	template <typename T>
	T* shared_ptr<T>::operator->() const noexcept
	{
	}

	template <typename T>
	long shared_ptr<T>::use_count() const noexcept
	{
	}

	template <typename T>
	bool shared_ptr<T>::unique() const noexcept
	{
	}

	template <typename T>
	shared_ptr<T>::operator bool() const noexcept
	{
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
	
	template<typename T>
	class enable_shared_from_this
	{
		
	};

} //namespace nimistl

#endif //MINISTL_SHARED_PTR_HPP