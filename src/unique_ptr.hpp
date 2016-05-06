#ifndef MINISTL_UNIQUE_PTR_HPP
#define MINISTL_UNIQUE_PTR_HPP

#include "types.hpp"
#include "internal/pointer.hpp"
namespace ministl
{
	//default_delete simply uses delete or delete[].
	template <class T> struct default_delete {
		constexpr default_delete() noexcept = default;
		template <class U> default_delete(const default_delete<U>&) noexcept {}
		void operator()(T* ptr) const
		{
			delete ptr;
		}
	};
	//partial specialization for array.
	template <class T> struct default_delete<T[]> {
		constexpr default_delete() noexcept = default;
		template <class U> default_delete(const default_delete<U[]>&) noexcept {}
		template <class U> void operator()(U* ptr) const
		{
			delete[] ptr;
		}
	};





	template <class T, class D = default_delete<T>> class unique_ptr {
	public:
		typedef see below pointer;
		typedef T element_type;
		typedef D deleter_type;
		// 20.10.1.2.1, constructors
		constexpr unique_ptr() noexcept;
		explicit unique_ptr(pointer p) noexcept;
		unique_ptr(pointer p, see below d1) noexcept;
		unique_ptr(pointer p, see below d2) noexcept;
		unique_ptr(unique_ptr&& u) noexcept;
		constexpr unique_ptr(nullptr_t) noexcept
			: unique_ptr() { }
		template <class U, class E>
		unique_ptr(unique_ptr<U, E>&& u) noexcept;
		// 20.10.1.2.2, destructor
		~unique_ptr();

		// 20.10.1.2.3, assignment
		unique_ptr& operator=(unique_ptr&& u) noexcept;
		template <class U, class E> unique_ptr& operator=(unique_ptr<U, E>&& u) noexcept;
		unique_ptr& operator=(nullptr_t) noexcept;

		// 20.10.1.2.4, observers
		add_lvalue_reference_t<T> operator*() const;
		pointer operator->() const noexcept;
		pointer get() const noexcept;
		deleter_type& get_deleter() noexcept;
		const deleter_type& get_deleter() const noexcept;
		explicit operator bool() const noexcept;

		// 20.10.1.2.5 modifiers
		pointer release() noexcept;
		void reset(pointer p = pointer()) noexcept;
		void swap(unique_ptr& u) noexcept;

		// disable copy from lvalue
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;


} //ministl

#include "unique_ptr_template.hpp"
#endif //MINISTL_UNIQUE_PTR_HPP