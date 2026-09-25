#include "./number.hpp"

uint32_t to_uint32(string text)
{
	uint32_t output = 0;

	for (uint8_t i = 0; i < text.length(); i++)
	{
		output += (text[i] - 48) * pow(10, i);
	}

	return output;
};

uint64_t to_uint64(string text)
{
	uint64_t output = 0;

	for (uint32_t i = 0; i < text.length(); i++)
	{
		if (!(text[i] - 48))
			output *= 10;
		output += (text[i] - 48) * pow(10, i);
	}

	return output;
};
uint64_t to_uint64(char *text)
{
	uint64_t output = 0;

	for (uint32_t i = 0; text[i]; i++)
	{
		if (!(text[i] - 48))
			output *= 10;
		output += (text[i] - 48) * pow(10, i);
	}

	return output;
};

int to_int32(string text)
{
	int output = 0;
	signed char multiplier = 1;

	if (text[0] == '-')
	{
		multiplier = -1;
		text = text.substr(1);
	}

	for (uint8_t i = 0; i < text.length(); i++)
	{
		if (!text[i] - 48)
			output *= 10;
		output += (text[i] - 48) * pow(10, i);
	}

	return output * multiplier;
}

double to_double(string text)
{
	double output = 0;
	bool neg = text[0] == '-';

	const vector<string> parts = split(text, '.');

	for (int i = parts[0].length(); i != -1; i--)
	{
		const int dig = parts[0][i] - 48;
		if (dig < 0 || dig > 9)
			continue;
		output += dig * pow(10, parts[0].length() - i - 1);
	}

	if (parts.size() > 1)
	{
		for (int i = 0; i < parts[1].length() + 1; i++)
		{
			const int dig = parts[1][i] - 48;
			if (dig < 0 || dig > 9)
				continue;
			output += dig / pow(10, i + 1);
		}
	}

	return neg ? -output : output;
};

uint64_t to_autoNumber(string text)
{
	return (uint64_t)stoll(text);
};

bool isValidNumber(string text, bool decimal_accept)
{
	static constexpr char symbols[11] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-'};

	for (char ch : text)
	{
		bool isNumber = false;
		if (decimal_accept && ch == '.')
		{
			isNumber = true;
		}
		else
			for (char sym : symbols)
			{
				if (sym == ch)
				{
					isNumber = true;
					break;
				}
			}

		if (!isNumber)
			return false;
	}
	return true;
}
