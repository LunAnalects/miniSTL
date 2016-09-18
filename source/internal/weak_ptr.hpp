#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef MINISTL_WEAK_PTR_HPP
#define MINISTL_WEAK_PTR_HPP

#include "utility.hpp"
#include "shared_ptr.hpp"

namespace ministl
{
	class bad_weak_ptr: public std::exception
	{
		
	};

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
} //namespace nimistl

#endif //MINISTL_WEAK_PTR_HPP