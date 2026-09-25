#ifndef NUMBER_HPP
#define NUMBER_HPP
#include <cstdint>
#include <string>
#include <cmath>
#include <vector>

using std::pow;
using std::stoll;
using std::string;
using std::uint16_t;
using std::uint32_t;
using std::uint8_t;
using std::vector;

inline string filter(string text, char *list, int count)
{
	string output = "";
	for (int i = 0; i < text.length(); i++)
	{
		bool includes = false;
		for (int j = 0; j < count; j++)
		{
			if (text[i] == list[j])
			{
				includes = true;
				break;
			}
		}
		if (includes)
			continue;
		output += text[i];
	}
	return output;
}

inline vector<string> split(string text, char splitter)
{
	vector<string> output;
	string temp = "";

	for (char ch : text)
	{
		if (ch == splitter)
		{
			output.push_back(temp);
			temp.clear();
			continue;
		}
		temp += ch;
	}
	if (temp.length())
		output.push_back(temp);
	temp.clear();

	return output;
}

uint32_t to_uint32(string text);
uint64_t to_uint64(string text);
uint64_t to_autoNumber(string text);

int to_int32(string text);

inline uint16_t to_uint16(string text)
{
	return (uint16_t)to_uint32(text);
}

double to_double(string text);
bool isValidNumber(string text, bool decimal_accept = false);
inline uint32_t min(int num1, int num2)
{
	return num1 > num2 ? num2 : num1;
}
inline uint32_t max(int num1, int num2)
{
	return num1 > num2 ? num1 : num2;
}
inline long int mins64(long int num1, long int num2)
{
	return num1 > num2 ? num2 : num1;
}
inline long int maxs64(long int num1, long int num2)
{
	return num1 > num2 ? num1 : num2;
}
inline uint64_t minu64(uint64_t num1, uint64_t num2)
{
	return num1 > num2 ? num2 : num1;
}
inline uint64_t maxu64(uint64_t num1, uint64_t num2)
{
	return num1 > num2 ? num1 : num2;
}
#endif
