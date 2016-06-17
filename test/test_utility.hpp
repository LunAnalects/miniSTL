#ifndef MINISTL_TEST_UTILITY_HPP
#define MINISTL_TEST_UTILITY_HPP

#include "gtest/gtest.h"
#include <string>
#include <memory>
#include <numeric>
#include <iostream>
#include <chrono>
//ministl
#include "vector.hpp"
#include "memory.hpp"
//STL
#include <algorithm>
#include <functional>


//clock
#define TICKDEFINE	decltype(std::chrono::high_resolution_clock::now()) start;\
					decltype(start) end;\
					long long ticks

#define TICKBEGIN	start = std::chrono::high_resolution_clock::now()

#define TICKEND(a)	end = std::chrono::high_resolution_clock::now();\
					ticks = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();\
					std::cout << #a << ticks << " microseconds." << std::endl
					


//output
template<typename Container>
void ContainerOut(Container c)
{
	std::cout << "container:" << std::endl;
	for (auto i = c.begin(); i != c.end(); ++i)
		std::cout << i << "   ";
	std::cout << "------------" << std::endl;
}


template<typename... Args>
void Cout3(const char* name,Args... args)
{
	std::cout << "RESULT of"<<name<<": " ;
	prt(args...);
}

template<typename T1>
void prt(T1 t1)
{
	std::cout << " T1: "<<t1 << std::endl;
}

template<typename T1, typename T2>
void prt(T1 t1, T2 t2)
{
	std::cout << " T1: "<< t1 << " T2: " << t2 << std::endl;
}

template<typename T1, typename T2, typename T3>
void prt(T1 t1, T2 t2, T3 t3)
{
	std::cout << " T1: " << t1 << " T2: " << t2 << " T3: " << t3 << std::endl;
}


template<typename T>
class HasPtr
{
	using this_type = HasPtr<T>;
public:
	HasPtr():size(0),data(nullptr){}

	explicit HasPtr(size_t n):size(n),data(new T[n]){}

	HasPtr(const this_type& that)
		:size(that.size), data(static_cast<T*>(operator new(sizeof(T) * size)))
	{
		if (size == 0) data = nullptr;
		else
		{
			//using copy constructor to construct uninitialized array.
			std::uninitialized_copy(that.data, that.data + that.size, data);
		}

		std::cout << "HasPtr(Size:" << size << ") Copy Constructed" << std::endl;
	}

	//HasPtr(this_type&& rthat)
	//	:size(rthat.size), data(rthat.data)
	//{
	//	ministl::uninitialized_copy()
	//}

	this_type& operator=(const this_type& rhs) = delete;


	virtual ~HasPtr()
	{
		std::cout << "HasPtr(Size:" << size << ") Destructed" << std::endl;
		delete[] data;
	}
	T& operator[](size_t index)
	{
		return data[index];
	}
private:
	size_t size;
	T* data;
};

template<typename T, typename InitFun>
T* ArrayGen(size_t len, InitFun initfunction)
{
	T* arr = new T[len];
	initfunction(arr, arr+len);
}

int* ArrayGen(size_t len, int first = 0)
{
	int* arr = new int[len];
	std::iota(arr, arr + len, first);
	return arr;
}

#endif // MINISTL_TEST_UTILITY_HPP
