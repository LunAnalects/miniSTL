#ifndef MINISTL_INTERNAL_POINTER_HPP
#define MINISTL_INTERNAL_POINTER_HPP

#include <type_traits>

namespace ministl
{
	namespace internal
	{

	} //ministl::internal
	

	template<typename Ptr>
	struct pointer_traits {
		using pointer = Ptr;
		//Ptr::element_type if present. Otherwise T if Ptr is a template instantiation
	};

	template<typename T>
	struct pointer_traits<T*> {
		using pointer = T*;
		using element_type = T;
		using difference_type = ptrdiff_t;

		template<typename U>
		using rebind = U*;


	};


} //ministl


#endif //MINISTL_INTERNAL_POINTER_HPP