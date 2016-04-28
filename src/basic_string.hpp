#ifndef MINISTL_BASIC_STRING_HPP
#define MINISTL_BASIC_STRING_HPP
namespace ministl
{

	template<typename _CHR> class basic_string
	{
	public:
		basic_string();
		basic_string(_CHR, size_t t);
		basic_string(const _CHR*);
		basic_string(basic_string&);

		basic_string& operator=(const basic_string&);

		//not virtual detor, No inheritance.
		~basic_string();


		//operator
		basic_string& operator+(basic_string&);
		_CHR& operator[](basic_string&);

		//member functions
		basic_string& reserve(size_t cap);
		size_t length();

	private:
		void resize(size_t);
		_CHR* char_ptr;
		size_t cnt;
	};

	template <typename _CHR>
	basic_string<_CHR>::basic_string(basic_string&)
	{
	}

	template<typename _CHR>
	basic_string<_CHR>& basic_string<_CHR>::operator=(const basic_string&)
	{

	}

	template <typename _CHR>
	basic_string<_CHR>::~basic_string()
	{
	}

	template <typename _CHR>
	basic_string<_CHR>& basic_string<_CHR>::operator+(basic_string&)
	{
	}

	template <typename _CHR>
	_CHR& basic_string<_CHR>::operator[](basic_string&)
	{
	}


}//namespace ministl

#endif //MINISTL_BASIC_STRING_HPP
