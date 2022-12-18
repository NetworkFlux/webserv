#include "../include/utils.hpp"

/*	This functions checks whether the character c is in the string str.	*/
bool	charInStr(const char& c, const std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (c == str[i])
			return (true);
	}
	return (false);
}

std::string toString(const int& value)
{
	std::ostringstream oss;
	oss << value;
	return oss.str();
}