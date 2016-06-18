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
	//partial specialization for array and pointer.
	template <class T> struct default_delete<T[]> {
		constexpr default_delete() noexcept = default;
		template <class U> default_delete(const default_delete<U[]>&) noexcept {}
		template <class U> void operator()(U* ptr) const
		{
			delete[] ptr;
		}
	};



	//TODO: unique_ptr's pointer type
	//TODO: Specialization for unbounded arrays? class unique_ptr<T[], Deleter>
	template <class T, class D = default_delete<T>>
	class unique_ptr
	{
	public:
		typedef T* pointer;
		typedef T element_type;
		typedef D deleter_type;

	protected:
		pointer			uPtr_;
		deleter_type	uDeleter_;

	public:
		// 20.10.1.2.1, constructors
		constexpr unique_ptr() noexcept
			:uPtr_(nullptr)
		{
		}

		explicit unique_ptr(pointer p) noexcept
			: uPtr_(p)
		{

		}


		unique_ptr(pointer p, deleter_type d1) noexcept
			: uPtr_(p), uDeleter_(d1)
		{

		}

		//unique_ptr(pointer p, ? d2) noexcept;
		unique_ptr(unique_ptr&& u) noexcept
			:uPtr_(u.get())
		{

		}
		constexpr unique_ptr(nullptr_t) noexcept
			: unique_ptr() { }


		template <class U, class E>
		unique_ptr(unique_ptr<U, E>&& u) noexcept
			: uPtr_(u.get()), uDeleter_(u.get_deleter())
		{

		}

		// 20.10.1.2.2, destructor //in standard this has no noexcept keyword
		~unique_ptr() noexcept
		{
			//former code:  uDeleter_(uPtr_); 
			reset();
		}

		// 20.10.1.2.3, move assignment
		unique_ptr& operator=(unique_ptr&& u) noexcept
		{
			auto tempptr = uPtr_;
			uPtr_ = u.get();
			uDeleter_(tempptr);
			return *this;
		}
		template <class U, class E>
		unique_ptr& operator=(unique_ptr<U, E>&& u) noexcept
		{
			auto tempptr = uPtr_;
			uPtr_ = u.get();
			uDeleter_ = u.get_deleter();
			return *this;
		}
		unique_ptr& operator=(nullptr_t) noexcept
		{
			uPtr_ = nullptr;
			return *this;
		}

		// 20.10.1.2.4, observers
		//TODO:add_lvalue_reference_t
		//add_lvalue_reference_t<T> operator*() const;
		T&					operator*() const
		{
			return *uPtr_;
		}

		pointer				operator->() const noexcept
		{
			return &this->operator*();
		}

		pointer				get() const noexcept
		{
			return uPtr_;
		}
		deleter_type&		get_deleter() noexcept
		{
			return uDeleter_;
		}
		const deleter_type& get_deleter() const noexcept
		{
			return uDeleter_;
		}
		explicit			operator bool() const noexcept
		{
			return uPtr_ != pointer();
		}

		// 20.10.1.2.5 modifiers
		pointer				release() noexcept
		{
			const pointer pTemp = uPtr_;
			uPtr_ = pointer();
			return pTemp;
		}

		void				reset(pointer p = pointer()) noexcept
		{
			if (p != uPtr_)
			{
				uDeleter_(uPtr_);
				uPtr_ = p;
			}
		}

		void	swap(unique_ptr& u) noexcept
		{
			//TODO
		}


		// disable copy from lvalue
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;




	};//class unique_ptr

} //ministl
#endif //MINISTL_UNIQUE_PTR_HPP