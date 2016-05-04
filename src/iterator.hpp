#ifndef MINISTL_ITERATOR_HPP
#define MINISTL_ITERATOR_HPP

namespace ministl
{
	template<typename T>
	class iterator_traits
	{
		using value_type		= typename T::value_type;
		using difference_type	= typename T::difference_type;
		using pointer			= typename T::pointer;
		using reference			= typename T::reference;
		using iterator_category = typename T::iterator_category;

	};

	template<typename T>
	class iterator_traits<T *>
	{
		using value_type		= typename T::value_type;
		using difference_type	= typename T::difference_type;
		using pointer			= typename T::pointer;
		using reference			= typename T::reference;
		using iterator_category = typename T::iterator_category;

	};

	template<typename T>
	class iterator_traits<const T *>
	{
		using value_type		= typename T::value_type;
		using difference_type	= typename T::difference_type;
		using pointer			= typename T::pointer;
		using reference			= typename T::reference;
		using iterator_category = typename T::iterator_category;

	};
	
} //ministl


#endif //MINISTL_ITERATOR_HPP