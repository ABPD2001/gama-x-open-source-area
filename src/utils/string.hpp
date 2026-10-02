#ifndef STRING_HPP
#define STRING_HPP
#include <string>
#include <vector>
#include <cmath>
#include <cstdint>
#include <cstring>

using std::memcpy;
using std::pow;
using std::string;
using std::uint16_t;
using std::uint32_t;
using std::uint8_t;
using std::vector;

struct argument_t
{
	string value;
	string key;
};

extern bool isValidNumber(string text, bool decimal_accept);

bool is_valid_version(string text);

vector<argument_t> parse_arguments(string content);
bool includes(const char *arr, uint8_t count, char ch);
vector<string> split(string text, char splitter);
string join(vector<string> arr, string glue);
string trim(string text);
string replaceAll(string text, string from, string to);
string filter(string text, char from);
string filter(string text, char *chars, uint8_t count);
string filter(string text, const char *chars, uint8_t count);
bool valid_name(string text);

inline bool valid_string(string text)
{
	return text.find_first_of('"') != string::npos && text.find_first_of('"') != text.find_last_of('"');
};

string toBinary(long int value, uint8_t mode, bool bigEndian);
inline string toBinaryu64(uint64_t value, bool bigEndian)
{
	string output(8, 0);

	for (uint8_t i = 0; i < 8; ++i)
	{
		output[i] = static_cast<char>((value >> (i * 8)) & 0xFF);
	}

	if (bigEndian)
	{
		string reversed(8, 0);
		for (uint8_t i = 0; i < 8; ++i)
		{
			reversed[i] = output[7 - i];
		}
		return reversed;
	}

	return output;
}

inline string filter(string text, string from)
{
	return replaceAll(text, from, "");
}

inline string toUppercase(string text)
{
	string output;

	for (char ch : text)
	{
		if (ch >= 97 && ch <= 122)
		{
			output += ch - 32;
		}
	}

	return output;
}

inline string toLowercase(string text)
{
	string output;

	for (char ch : text)
	{
		if (ch >= 65 && ch <= 90)
		{
			output += ch + 32;
		}
	}

	return output;
}
#endif
