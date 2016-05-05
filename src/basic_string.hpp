#ifndef MINISTL_BASIC_STRING_HPP
#define MINISTL_BASIC_STRING_HPP

#include <cstring>

#include "types.hpp"
#include "allocator.hpp"

namespace ministl

{

	template<typename CharType>
	class char_traits
	{
		
	};

	template<> 
	class char_traits<char> 
	{
	public:
		typedef char char_type;
		typedef int int_type;
		//typedef streamoff off_type;
		//typedef streampos pos_type;
		//typedef mbstate_t state_type;
		static void assign(char_type& c1, const char_type& c2) noexcept;
		static constexpr bool eq(char_type c1, char_type c2) noexcept;
		static constexpr bool lt(char_type c1, char_type c2) noexcept;
		static int compare(const char_type* s1, const char_type* s2, size_t n);
		static size_t length(const char_type* s);
		static const char_type* find(const char_type* s, size_t n,
			const char_type& a);
		static char_type* move(char_type* s1, const char_type* s2, size_t n);
		static char_type* copy(char_type* s1, const char_type* s2, size_t n);
		static char_type* assign(char_type* s, size_t n, char_type a);
		static constexpr int_type not_eof(int_type c) noexcept;
		static constexpr char_type to_char_type(int_type c) noexcept;
		static constexpr int_type to_int_type(char_type c) noexcept;
		static constexpr bool eq_int_type(int_type c1, int_type c2) noexcept;
		static constexpr int_type eof() noexcept;
	};

	template<typename charT, 
		typename Allocator = allocator<charT>> 
	class basic_string
	{

	public:
		using this_type			= basic_string<charT, Allocator>;
		using value_type		= charT;
		using pointer			= charT *;
		using reference			= charT &;
		using const_pointer		= const charT *;
		using const_reference	= const charT &;
		using size_type			= size_t;
		using allocator_type	= Allocator;
		using iterator			= pointer;
		using const_iterator	= const_pointer;

	public:
		static const size_type npos = static_cast<size_type>(-1) ;

	public:
		//copy control
		explicit basic_string(const Allocator& a) noexcept:
		{

		}


		basic_string():char_ptr(nullptr), char_begin(0), char_end(0), cap(0)
		{
			
		}

		basic_string(value_type, size_type t):char_begin(0), char_end(t), cap(t+1)
		{
			char_resize(t);

		}
		explicit basic_string(const_pointer cstr)
		{
			size_type cstr_len = strlen(cstr);
			cnt = cstr_len + 1;
			cap = cstr_len + 1;
			
			resize(cap);

			
		}
		basic_string(const this_type &str1):cnt(str1.cnt), cap(str1.cap)
		{
			char_resize(cap);

		}
		//copy and swap
		this_type& operator=(const this_type&)
		{
			 
		}


		this_type& operator=(const_pointer cstr)
		{

		}

		//not virtual detor, No inheritance.
		~basic_string()
		{
			char_delete();
		}


		//operator
		this_type& operator+(this_type&)
		{

		}
		value_type& operator[](this_type&)
		{
			
		}




		//member functions
		///Capacity
		this_type& reserve(size_type cap)
		{
			if (cnt != 0)
				char_delete();
			char_resize(cap);
		}
		size_type length() const
		{
			return char_end - char_begin;
		}
		size_type max_size() const
		{
			return cap;
		}
		size_type capacity() const
		{
			return cap;
		}
		void shrink_to_fit()
		{
			
		}

		///Element access
		reference at(size_type index)
		{
			return char_ptr[index];
		}
		
		///Operations

		void clear() noexcept;
		this_type &insert(size_type index, size_type count, value_type ch);
		this_type &insert( size_type index, const_pointer s);
		this_type &insert( size_type index, const_pointer, size_type count);
		this_type &insert( size_type index, const this_type &str);
		this_type &insert( size_type index, const this_type & str
			size_type index_str, size_type count = npos);

		void push_back(value_type ch);


		this_type &append(size_type count, value_type ch);
		this_type &append(const this_type &str);
		this_type &append(const_pointer s, size_type count);
		this_type &append(const_pointer s);

		this_type &operator+=(const this_type &str);
		this_type &operator+=(value_type ch);
		this_type &operator+=(const_pointer s);	
		//TODO initializer_list<value_type> ilist


		int compare(const basic_string& str) const;
		int compare(size_type pos1, size_type count1,
			const basic_string& str) const;
		int compare(size_type pos1, size_type count1,
			const basic_string& str,
			size_type pos2, size_type count2 = npos) const;
		int compare(const CharT* s) const;


		basic_string& replace(size_type pos1, size_type n1,
			const basic_string& str,
			size_type pos2, size_type n2 = npos);
		basic_string& replace(size_type pos, size_type n1, const charT* s,
			size_type n2);
		basic_string& replace(size_type pos, size_type n1, const charT* s);
		basic_string& replace(size_type pos, size_type n1, size_type n2,
			charT c);
		basic_string& replace(const_iterator i1, const_iterator i2,
			const basic_string& str);
		basic_string& replace(const_iterator i1, const_iterator i2, const charT* s,
			size_type n);
		basic_string& replace(const_iterator i1, const_iterator i2, const charT* s);
		basic_string& replace(const_iterator i1, const_iterator i2,
			size_type n, charT c);
		//template<class InputIterator>
		//basic_string& replace(const_iterator i1, const_iterator i2,
		//	InputIterator j1, InputIterator j2);
		//basic_string& replace(const_iterator, const_iterator, initializer_list<charT>);
		//size_type copy(charT* s, size_type n, size_type pos = 0) const;
		//void swap(basic_string& str)
		//	noexcept(allocator_traits<Allocator>::propagate_on_container_swap::value ||
		//		allocator_traits<Allocator>::is_always_equal::value);


		/// Search


	private:
		allocator_type alloc;
		pointer char_ptr;
		size_type char_begin;
		size_type char_end;
		size_type cap;
		void char_delete()
		{
			alloc.deallocate(char_ptr);
		}

		void char_resize(size_type t)
		{
			if (t > cap) 
			{
				char_delete();
				char_ptr = alloc.allocate(t + 1);
			}
		}

		

	};

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::
		insert(size_type index, size_type count, value_type ch)
	{

	}

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::
		insert(size_type index, const_pointer s)
	{

	}

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::
		insert(size_type index, const_pointer, size_type count)
	{

	}

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::
		insert(size_type index, const this_type &str)
	{

	}

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::
		insert(size_type index, const this_type & str, size_type index_str, size_type count = npos)
	{

	}

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::append(size_type count, value_type ch)
	{
	}

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::append(const this_type& str)
	{
	}

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::append(const_pointer s, size_type count)
	{
	}

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::append(const_pointer s)
	{
	}

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::operator+=(const this_type& str)
	{
	}

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::operator+=(value_type ch)
	{
	}

	template <typename charT, typename Allocator>
	typename basic_string<charT, Allocator>::this_type& basic_string<charT, Allocator>::operator+=(const_pointer s)
	{
	}

	template <typename charT, typename Allocator>
	int basic_string<charT, Allocator>::compare(const basic_string& str) const
	{
	}

	template <typename charT, typename Allocator>
	int basic_string<charT, Allocator>::compare(size_type pos1, size_type count1, const basic_string& str) const
	{
	}

	template <typename charT, typename Allocator>
	int basic_string<charT, Allocator>::compare(size_type pos1, size_type count1, const basic_string& str, size_type pos2, size_type count2) const
	{
	}

	template <typename charT, typename Allocator>
	basic_string<charT, Allocator>& basic_string<charT, Allocator>::replace(size_type pos1, size_type n1, const basic_string& str, size_type pos2, size_type n2)
	{
	}

	template <typename charT, typename Allocator>
	basic_string<charT, Allocator>& basic_string<charT, Allocator>::replace(size_type pos, size_type n1, const charT* s, size_type n2)
	{
	}

	template <typename charT, typename Allocator>
	basic_string<charT, Allocator>& basic_string<charT, Allocator>::replace(size_type pos, size_type n1, const charT* s)
	{
	}

	template <typename charT, typename Allocator>
	basic_string<charT, Allocator>& basic_string<charT, Allocator>::replace(size_type pos, size_type n1, size_type n2, charT c)
	{
	}

	template <typename charT, typename Allocator>
	basic_string<charT, Allocator>& basic_string<charT, Allocator>::replace(const_iterator i1, const_iterator i2, const basic_string& str)
	{
	}

	template <typename charT, typename Allocator>
	basic_string<charT, Allocator>& basic_string<charT, Allocator>::replace(const_iterator i1, const_iterator i2, const charT* s, size_type n)
	{
	}

	template <typename charT, typename Allocator>
	basic_string<charT, Allocator>& basic_string<charT, Allocator>::replace(const_iterator i1, const_iterator i2, const charT* s)
	{
	}

	template <typename charT, typename Allocator>
	basic_string<charT, Allocator>& basic_string<charT, Allocator>::replace(const_iterator i1, const_iterator i2, size_type n, charT c)
	{
	}
}//namespace ministl


#endif //MINISTL_BASIC_STRING_HPP
