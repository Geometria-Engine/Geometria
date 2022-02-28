#pragma once

#ifndef STRING_API_H
#define STRING_API_H
#include <string>
#include <vector>

struct StringAPI
{
	static std::string ReplaceAll(std::string str, const std::string& from, const std::string& to);
	static std::string RemoveAll(std::string str, const std::string& del);
	static std::string GetSubstringBetween(std::string text, std::string first, std::string last);
	static std::vector<std::string> SplitIntoVector(const std::string& s, const char* c);
	static std::vector<std::string> GetLinesFromString(std::string s);
	static bool StartsWith(std::string string, std::string starts);
	static bool EndsWith(std::string string, std::string ends);
};
#endif
