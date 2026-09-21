#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <string>
#include <vector>
#include <cstdint>

using std::string;
using std::uint8_t;
using std::vector;

template <typename T = string>
int find(vector<T> arr, T elm)
{
	for (uint32_t i = 0; i < arr.size(); i++)
	{
		if (arr[i] == elm)
			return i;
	}
	return -1;
};

template <typename T = string, uint32_t size>
bool includes_arr(T *arr, T element)
{
	for (uint32_t i = 0; i < size; i++)
	{
		if (arr[i] == element)
			return true;
	}
	return false;
}
template <typename T = string, uint32_t size>
int find_arr(T *arr, T element)
{
	for (uint32_t i = 0; i < size; i++)
	{
		if (arr[i] == element)
			return i;
	}
	return -1;
}

template <typename T = string>
bool includes(vector<T> vec, T element)
{
	if (!vec.size())
		return false;
	for (uint32_t i = 0; i < vec.size(); i++)
	{
		if (vec[i] == element)
			return true;
	}
	return false;
}

#endif
