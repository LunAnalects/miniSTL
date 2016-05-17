#ifndef MINISTL_COPY_HPP
#define MINISTL_COPY_HPP
#include <type_traits>
using std::remove_reference;

namespace ministl
{
	//This implementation of move() is from C++Primer.
	template <typename T>
	typename std::remove_reference<T>&& move(T&& t)
	{
		return static_cast<typename remove_reference<T>&&>(t);
	}


} //namespace ministl

#endif //MINISTL_COPY_HPP