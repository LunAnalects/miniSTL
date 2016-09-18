#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


#ifndef MINISTL_TYPE_TRAITS_HPP
#define MINISTL_TYPE_TRAITS_HPP

namespace ministl
{
	///////////////////////////////////////////////////////////////////////
	// integral_constant
	//
	// This is the base class for various type traits, as defined by C++11. 
	// This is essentially a utility base class for defining properties
	// as both class constants (value) and as types (type).
	//
	template <typename T, T v>
	struct integral_constant
	{
		static const T value = v;
		typedef T value_type;
		typedef integral_constant<T, v> type;
	};
	///////////////////////////////////////////////////////////////////////
	// true_type / false_type
	//
	// These are commonly used types in the implementation of type_traits.
	// Other integral constant types can be defined, such as those based on int.
	//
	typedef integral_constant<bool, true>  true_type;
	typedef integral_constant<bool, false> false_type;


	///////////////////////////////////////////////////////////////////////
	// yes_type / no_type
	//
	// These are used as a utility to differentiate between two things.
	//
	typedef char yes_type;                      // sizeof(yes_type) == 1
	struct       no_type { char padding[8]; };  // sizeof(no_type)  != 1


	///////////////////////////////////////////////////////////////////////
	// empty
	//
	template <typename T>
	struct empty { };


	///////////////////////////////////////////////////////////////////////
	// unused
	//
	// Used internally to denote a special template argument that means 
	// it's an unused argument.
	//
	struct unused { };


	///////////////////////////////////////////////////////////////////////
	// argument_sink
	//
	// Used as a type which constructs from anything. 
	//

	// For compilers that support variadic templates we provide an
	// alternative argument_sink which provides a constructor overload of
	// the variadic pack of arguments by reference.  This avoids issues of
	// object alignment not being respected in Microsoft compilers.  Seen
	// in VS2015 preview.  In general, since arguments are consumed and
	// ignored its cheaper to consume references than passing by value
	// which incurs a construction cost.
	struct argument_sink
	{
		template<typename... Args>
		argument_sink(Args&&...) {}
	};

	///////////////////////////////////////////////////////////////////////
	// type_select
	//
	// This is used to declare a type from one of two type options. 
	// The result is based on the condition type. This has certain uses
	// in template metaprogramming.
	//
	// Example usage:
	//    typedef ChosenType = typename type_select<is_integral<SomeType>::value, ChoiceAType, ChoiceBType>::type;
	//
	template <bool bCondition, class ConditionIsTrueType, class ConditionIsFalseType>
	struct type_select { typedef ConditionIsTrueType type; };

	template <typename ConditionIsTrueType, class ConditionIsFalseType>
	struct type_select<false, ConditionIsTrueType, ConditionIsFalseType> { typedef ConditionIsFalseType type; };


	///////////////////////////////////////////////////////////////////////
	// first_type_select
	//
	//  Similar to type_select but unilaterally selects the first type.
	//
	template <typename T, typename = ministl::unused, typename = ministl::unused>
	struct first_type_select { typedef T type; };



	///////////////////////////////////////////////////////////////////////
	// type_or
	//
	// This is a utility class for creating composite type traits.
	//
	template <bool b1, bool b2, bool b3 = false, bool b4 = false, bool b5 = false>
	struct type_or;

	template <bool b1, bool b2, bool b3, bool b4, bool b5>
	struct type_or { static const bool value = true; };

	template <>
	struct type_or<false, false, false, false, false> { static const bool value = false; };



	///////////////////////////////////////////////////////////////////////
	// type_and
	//
	// This is a utility class for creating composite type traits.
	//
	template <bool b1, bool b2, bool b3 = true, bool b4 = true, bool b5 = true>
	struct type_and;

	template <bool b1, bool b2, bool b3, bool b4, bool b5>
	struct type_and { static const bool value = false; };

	template <>
	struct type_and<true, true, true, true, true> { static const bool value = true; };



	///////////////////////////////////////////////////////////////////////
	// type_equal
	//
	// This is a utility class for creating composite type traits.
	//
	template <int b1, int b2>
	struct type_equal { static const bool value = (b1 == b2); };



	///////////////////////////////////////////////////////////////////////
	// type_not_equal
	//
	// This is a utility class for creating composite type traits.
	//
	template <int b1, int b2>
	struct type_not_equal { static const bool value = (b1 != b2); };



	///////////////////////////////////////////////////////////////////////
	// type_not
	//
	// This is a utility class for creating composite type traits.
	//
	template <bool b>
	struct type_not { static const bool value = true; };

	template <>
	struct type_not<true> { static const bool value = false; };



	///////////////////////////////////////////////////////////////////////
	// enable_if, disable_if
	//
	// template <bool B, typename T = void> struct enable_if;
	// template <bool B, typename T = void> struct disable_if;

	template<bool B, typename T = void>
	struct enable_if {};

	template <typename T>
	struct enable_if<true, T> { typedef T type; };


	template<bool B, typename T = void>
	struct disable_if {};

	template <typename T>
	struct disable_if<false, T> { typedef T type; };





	//const
	template <typename T>           
	struct remove_const { typedef T type; };

	template <typename T>           
	struct remove_const<const T> { typedef T type; };

	template <typename T>           
	struct remove_const<const T[]> { typedef T type[]; };

	template <typename T, size_t N> 
	struct remove_const<const T[N]> { typedef T type[N]; };
	//volatile
	template <typename T>
	struct remove_volatile { typedef T type; };

	template <typename T>
	struct remove_volatile<volatile T> { typedef T type; };

	template <typename T>
	struct remove_volatile<volatile T[]> { typedef T type[]; };

	template <typename T, size_t N>
	struct remove_volatile<volatile T[N]> { typedef T type[N]; };
	//volatile and const
	template <typename T>
	struct remove_cv
	{
		typedef typename ministl::remove_volatile<typename ministl::remove_const<T>::type>::type type;
	};
	//helper
	template< typename T >
	using remove_cv_t = typename remove_cv<T>::type;
	
	template< typename T >
	using remove_const_t = typename remove_const<T>::type;
	
	template< typename T >
	using remove_volatile_t = typename remove_volatile<T>::type;



	//remove_reference
	template<typename T>
	struct remove_reference { typedef T value_type; };
	template<typename T>
	struct remove_reference<T&> { typedef T value_type; };
	template<typename T>
	struct remove_reference<T&&> { typedef T value_type; };
} //ministl


#endif //MINISTL_TYPE_TRAITS_HPP