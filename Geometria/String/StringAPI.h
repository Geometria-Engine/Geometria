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

	struct LoremIpsum
	{
		// Original Source from: https://www.lipsum.com/

		static std::string InfoSource()
		{
			return "https://www.lipsum.com/";
		}

		static std::string WhatIsIt()
		{
			return "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum";
		}

		static std::string Original()
		{
			return "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
		}
	};
};
#endif
