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
	static bool IsOnlyDigits(const std::string &str)
	{
	    return str.find_first_not_of("0123456789") == std::string::npos;
	}
	static bool ContainsDigits(const std::string &str)
	{
		return str.find_first_of("0123456789") != std::string::npos;
	}
	static bool IsOnlyLetters(const std::string &str)
	{
	    return str.find_first_not_of("abcdefghijklmnopqrstuvwxyz") == std::string::npos;
	}
	static bool ContainsLetters(const std::string &str)
	{
		return str.find_first_of("abcdefghijklmnopqrstuvwxyz") != std::string::npos;
	}
};
#endif
