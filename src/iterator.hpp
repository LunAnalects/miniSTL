#ifndef MINISTL_ITERATOR_HPP
#define MINISTL_ITERATOR_HPP
#include "types.hpp"

/*
Layout in this header:

//iterator_tag
//iterator_traits


//advance()

*/
namespace ministl
{

	
	//five kinds of iterators.
	class input_iterator_tag												{};
	class output_iterator_tag												{};
	class forward_iterator_tag :		public input_iterator_tag			{};
	class bidirectional_iterator_tag:	public forward_iterator_tag			{};
	class random_access_iterator_tag:	public bidirectional_iterator_tag	{};

	//iterator_traits(partial specializations for pointer and const pointer).
	template<typename T>
	class iterator_traits
	{
	public:
		using iterator_category = typename T::iterator_category;
		using value_type		= typename T::value_type;
		using difference_type	= typename T::difference_type;
		using pointer			= typename T::pointer;
		using reference			= typename T::reference;

	};

	template<typename T>
	class iterator_traits<T *>
	{
	public:
		using iterator_category =  random_access_iterator_tag;
		using value_type		= T;
		using difference_type	=  ptrdiff_t;
		using pointer			=  T *;
		using reference			=  T &;

	};

	template<typename T>
	class iterator_traits<const T *>
	{
	public:
		using iterator_category =  random_access_iterator_tag;
		using value_type		=  T;
		using difference_type	=  ptrdiff_t;
		using pointer			=  T *;
		using reference			=  T &;

	};

	template<
			typename Category,
			typename T, 
			typename Distance	= ptrdiff_t,
			typename Pointer	= T*, 
			typename Reference	= T& 
			>
	class iterator
	{
	public:
		using iterator_category = Category;
		using value_type		= T;
		using difference_type	= Distance;
		using pointer			= Pointer;
		using reference			= Reference;
	};


	//template<typename T>
	//class input_iterator: public iterator<
	//	input_iterator_tag,
	//	typename iterator_traits<T>::value_type, 
	//	typename iterator_traits<T>::difference_type, 
	//	typename iterator_traits<T>::pointer, 
	//	typename iterator_traits<T>::reference>
	//{
	//	
	//};

	//template<typename T>
	//class output_iterator: public iterator<
	//	output_iterator_tag,
	//	typename iterator_traits<T>::value_type,
	//	typename iterator_traits<T>::difference_type,
	//	typename iterator_traits<T>::pointer,
	//	typename iterator_traits<T>::reference> 
	//{
	//	
	//};

	//class forward_iterator: public input_iterator<>
	//{
	//	
	//};


	//class bidirectional_iterator: public forward_iterator
	//{
	//	
	//};


	//class random_access_iterator :public bidirectional_iterator
	//{
	//	
	//};


	//when passed forward_iterator_tag() as the third argument, it will invoke this version due to inheritance. 
	template<typename InputIterator, typename Distance>
	inline void __advance_imp(InputIterator& i, Distance n,
		input_iterator_tag)
	{
		while (n--) ++i;
	}

	template<typename BidirectionalIterator, typename Distance>
	inline void __advance_imp(BidirectionalIterator& i, Distance n,
		bidirectional_iterator_tag)
	{
		if(n > 0)
		{
			while (n--) ++i;
		}
		else
		{
			while (n++) --i;
		}
	}
	template<typename RandomAccessIterator, typename Distance>
	inline void __advance_imp(RandomAccessIterator& i, Distance n,
		random_access_iterator_tag)
	{
		i += n;
	}

	//Template Argument Naming Rule: 
	//typename InputIterator means it can accept input_iterator and any other iterators in category above input_iterator(like random_access_iterator)
	template<typename InputIterator, typename Distance>
	inline void advance(InputIterator it, Distance n)
	{
		__advance_imp(it, n, iterator_traits<InputIterator>::iterator_category());
	}

} //ministl



#endif //MINISTL_ITERATOR_HPP