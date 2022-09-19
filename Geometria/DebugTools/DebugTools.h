#ifndef DEBUGTOOLS_H
#define DEBUGTOOLS_H

#include <signal.h>
#include "Files/Files.h"
#include "Multithreading/Multithreading.h"
#include <cstdlib>

#define GDT_ASSERT(x)\
{\
 	if(!(x))\
 	{\
 		std::cerr << "[DebugTools] ASSERT FAILED: " << #x << " @ " << __FILE__ << " (line: " << __LINE__ << ")" << std::endl;\
 		Graphics::Exit();\
 	}\
}

struct DebugTools
{

	static void SignalHandler(int signal);

};

#define GDT_WATCHPOINT_INIT()\
{\
	typedef void (*SignalHandlerPointer)(int);\
    SignalHandlerPointer previousHandler;\
    previousHandler = signal(SIGSEGV, DebugTools::SignalHandler);\
}

#define GDT_WATCHPOINT_START() std::cout << "[DebugTools] START WATCHPOINT: " << __FILE__ << " (line: " << __LINE__ << ")" << std::endl;  try {
#define GDT_WATCHPOINT_END()\
}\
catch (char* e)\
{\
    printf("Exception Caught: %s\n", e);\
    exit(1);\
}\
std::cout << "[DebugTools] END WATCHPOINT: " << __FILE__ << " (line: " << __LINE__ << ")" << std::endl;

#define _GEO_COMPILER(x) x

struct _Geometria_CoreLog
{
	static std::vector<std::string> nullptrValues;
	static std::string currentText;

	static bool CheckForExplicitNull(std::string line)
	{
		if(line.find("= nullptr;") != std::string::npos)
			return true;
		else if(line.find("= 0;") != std::string::npos)
			return true;
		else if(line.find("= NULL;") != std::string::npos)
			return true;

		return false;
	}

	static std::string NumberToSpace(std::string number)
	{
		std::string finalNumber = StringAPI::RemoveAll(number, "|");

		std::string result;
		for(int i = 0; i < finalNumber.size(); i++)
		{
			result += " ";
		}

		result += "|";
		return result;
	}

	static std::string SetVariablePoint(std::string line, std::string variable)
	{
		std::string result;

		if(line.find(variable) != std::string::npos)
		{
			for(int i = 0; i < line.find(variable); i++)
			{
				result += " ";
			}

			int varBegin = line.find(variable);
			if(varBegin - 1 != std::string::npos)
			{
				if(variable[varBegin - 1] == '*' || variable[varBegin - 1] == '&')
				{
					result += " ";
				}
			}
		}

		for(int i = 0; i < variable.size(); i++)
		{
			result += "^";
		}

		return result;
	}

	static std::string FindVarInNullptrList(std::string line)
	{
		for(auto i : nullptrValues)
		{
			if(line.find(i) != std::string::npos)
			{
				return i;
			}
		}

		return "";
	}

	static void WriteCurrentLine(std::string line, std::string file, int number)
	{
		if(line.find_first_of("*") != std::string::npos)
		{
			bool containsExplicitNull = _Geometria_CoreLog::CheckForExplicitNull(line);
			if(line.find_first_of("+-/") == std::string::npos &&
			 line.find("(") == std::string::npos &&
			 line.find(")") == std::string::npos && 
			 line.find("new") == std::string::npos && 
			 line.find("<<") == std::string::npos &&
			 line.find(">>") == std::string::npos &&
			 containsExplicitNull)
			{
				std::string getValue = "";

				if(containsExplicitNull)
					getValue = StringAPI::GetSubstringBetween(line, "*", " =");
				else
					getValue = StringAPI::GetSubstringBetween(line, "*", ";");

				getValue = StringAPI::GetSubstringBetween(getValue, " ", "");

				if(getValue.find_first_of("*") != std::string::npos)
					getValue = StringAPI::RemoveAll(getValue, "*");

				bool repeated = false;
				for(auto i : nullptrValues)
				{
					if(i == getValue)
					{
						repeated = true;
						break;
					}
				}

				if(!repeated)
				{
					nullptrValues.push_back(getValue);
					std::cout << "Nullptr result = " << getValue << std::endl;
				}
			}
		}

		std::string text = "";
		text += "  --> " + file + ":" + std::to_string(number) + "\n";
		text += NumberToSpace(std::to_string(number) + " |") + "\n";
		text += std::to_string(number) + " | " + line + "\n";
		text += NumberToSpace(std::to_string(number) + " |") + SetVariablePoint(line, FindVarInNullptrList(line)) + "\n";

		text += "={}=\n";

		std::string nullptrListStr = "";

		text += "NullptrList:[";

		for(auto i : nullptrValues)
		{
			text += i + "|";
		}

		text += "]";

		currentText = text;
	}
};

#endif