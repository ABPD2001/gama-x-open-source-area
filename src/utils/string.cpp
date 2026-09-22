#include "./string.hpp"

string toBinary(uint64_t value, uint8_t mode, bool bigEndian)
{
	if (mode > sizeof(value))
	{
		mode = sizeof(value);
	}

	string output(mode, 0);
	for (uint8_t i = 0; i < mode; ++i)
	{
		output[i] = static_cast<char>((value >> (i * 8)) & 0xFF);
	}

	string reversed(mode, 0);
	for (uint32_t i = mode - 1; i < mode; i--)
	{
		reversed[mode - 1 - i] = output[i];
	}

	return !bigEndian ? output : reversed;
}

bool is_valid_version(string text)
{
	const vector<string> parts = split(text, '.');
	if (parts.size() != 3)
		return false;
	if (!(isValidNumber(parts[0], false) && isValidNumber(parts[1], false) && isValidNumber(parts[2], false)))
		return false;
	if (parts[0].length() > 2 || parts[1].length() > 3 || parts[2].length() > 5)
		return false;
	return true;
}

vector<string> split(string text, char splitter)
{
	vector<string> output;
	string temp;

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
	output.push_back(temp);
	temp.clear();

	return output;
}

vector<argument_t> parse_arguments(string content)
{
	vector<argument_t> output;
	const vector<string> lines = split(content, '\n');

	for (string l : lines)
	{
		l = split(l, '#')[0];
		for (uint32_t i = 0; i < l.length(); i++)
		{
			argument_t argument;
			bool skip = false;

			if (l[i] == '=')
			{
				const string key = trim(l.substr(0, i));
				const string value = trim(l.substr(i + 1, l.length()));

				for (argument_t arg : output)
				{
					if (arg.key == key)
					{
						arg.value = value;
						break;
					}
				}
				argument = {value, key};
				output.push_back(argument);
			}
			if (skip)
				continue;
		}
	}

	return output;
}

string join(vector<string> arr, string glue)
{
	string output = "";
	for (uint32_t i = 0; i < arr.size(); i++)
	{
		output += arr[i];
		if (i != arr.size() - 1)
			output += glue;
	}
	return output;
}

bool includes(char *arr, uint8_t count, char ch)
{
	for (uint8_t i = 0; i < count; i++)
	{
		if (arr[i] == ch)
			return true;
	}
	return false;
}

bool includes(const char *arr, uint8_t count, char ch)
{
	for (uint8_t i = 0; i < count; i++)
	{
		if (arr[i] == ch)
			return true;
	}
	return false;
}

string trim(string text)
{
	string output = "";
	signed long int start = -1;
	signed long int end = -1;
	static constexpr char empty_chars[8] = {'\n', '\t', '\r', ' ', '\b', '\f', '\a', '\v'};

	for (uint32_t i = 0; i < text.length(); i++)
	{
		if (start == -1 && !includes(empty_chars, 8, text[i]))
		{
			start = i;
			break;
		}
	}

	for (int i = text.length() - 1; i > -1; i--)
	{
		if (end == -1 && !includes(empty_chars, 7, text[i]))
		{
			end = i;
			break;
		}
	}
	if (end == -1 || start == -1)
		return "";

	output = text.substr(start, end - start + 1);
	string cpy = "";
	bool spaced = false;

	for (uint32_t i = 0; i < output.length(); i++)
	{
		if (includes(empty_chars, 7, output[i]))
		{
			if (!spaced)
			{
				spaced = true;
				cpy += output[i];
			}
			else
				continue;
		}
		else
		{
			spaced = false;
			cpy += output[i];
		}
	}

	return cpy;
}

bool valid_name(string text)
{
	static constexpr char label_filter[8] = {'\r', '\n', ' ', '\a', '\b', '\t', '\f', ':'};

	for (char ch : label_filter)
	{
		if (text.find(ch) != string::npos)
			return false;
	}
	return true;
};

string replaceAll(string text, string from, string to)
{
	string cpy = text;

	while (cpy.find(from) != string::npos)
	{
		cpy.replace(cpy.find(from), from.length(), to);
	}

	return cpy;
}

string filter(string text, char from)
{
	string output = "";
	for (char ch : text)
	{
		if (ch != from)
			output += ch;
	}
	return output;
}

string filter(string text, char *chars, uint8_t count)
{
	string output = "";
	for (char ch : text)
	{
		if (includes(chars, count, ch))
			continue;
		output += ch;
	}
	return output;
}

string filter(string text, const char *chars, uint8_t count)
{
	string output = "";
	for (char ch : text)
	{
		bool found = false;
		for (uint8_t i = 0; i < count; i++)
		{
			if (chars[i] == ch)
				found = true;
		}
		if (found)
			continue;

		output += ch;
	}
	return output;
}