#include "StringAPI.h"
#include <sstream>
#include <cstring>

std::string StringAPI::ReplaceAll(std::string str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;
	std::string modifyString = str;
	while ((start_pos = modifyString.find(from, start_pos)) != std::string::npos)
	{
		modifyString.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
	return modifyString;
}

std::string StringAPI::RemoveAll(std::string str, const std::string& del)
{
	return StringAPI::ReplaceAll(str, del, "");
}

std::string StringAPI::GetSubstringBetween(std::string text, std::string firstT, std::string lastT)
{
	unsigned first = text.find(firstT);
	unsigned last = text.find(lastT);
	std::string level;
	if (first != -1 && last != -1)
	{
		level = text.substr(first, last - first);
		level = ReplaceAll(level, firstT, "");
	}
    return level;
}

std::vector<std::string> StringAPI::SplitIntoVector(const std::string& s, const char* c)
{
	std::vector<std::string> newV;
	std::string::size_type i = 0;
	std::string::size_type j = s.find(c);

	while (j != std::string::npos)
	{
		newV.push_back(s.substr(i, j - i));
		i = ++j;
		j = s.find(c, j);

		if (j == std::string::npos)
			newV.push_back(s.substr(i, s.length()));
	}

	if (newV.size() == 0)
		newV.push_back(s);

	return newV;
}

std::vector<std::string> StringAPI::GetLinesFromString(std::string s)
{
	std::vector<std::string> str;
	std::istringstream f(s);
	std::string line;
	while (std::getline(f, line)) 
	{
		str.push_back(line);
	}

	return str;
}

bool StringAPI::StartsWith(std::string string, std::string starts)
{
	if (strncmp(string.c_str(), starts.c_str(), strlen(starts.c_str())) == 0) return 1;
	return 0;
}

bool StringAPI::EndsWith(std::string string, std::string ends)
{
	return string.size() >= ends.size() && 0 == string.compare(ends.size() - ends.size(), ends.size(), ends);
}
