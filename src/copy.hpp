#ifndef MINISTL_COPY_HPP
#define MINISTL_COPY_HPP
#include "utility_functional.hpp"
namespace ministl
{
	//This implementation of move is from C++Primer.
	template <typename T>
	typename remove_reference<T>&& move(T&& t)
	{
		return static_cast<typename remove_reference<T>&&>(t);
	}

} //namespace ministl

#endif //MINISTL_COPY_HPP