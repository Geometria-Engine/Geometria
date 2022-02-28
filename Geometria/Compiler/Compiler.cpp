#include "Compiler.h"
#include "../Files/Files.h"
#include "../Graphics/Cores/MainAPI/Graphics.h"
#include <istream>
#include <iostream>
#include <string>

bool hasEnding(std::string const& fullString, std::string const& ending) {
	if (fullString.length() >= ending.length()) {
		return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
	}
	else {
		return false;
	}
}

std::string FillCommandSpaces(std::string text)
{
	std::string l, finalResult;
	std::istringstream cStream(text);
	bool isFirst = true;
	while (std::getline(cStream, l, '/'))
	{
		std::string changedLine;
		if (l.find(" ") != std::string::npos)
		{
			if (!isFirst)
				changedLine += "/";

			changedLine += "\"";
			changedLine += l;
			changedLine += "\"";
		}
		else
		{
			if (!isFirst)
				changedLine += "/";

			changedLine += l;
		}

		isFirst = false;
		finalResult += changedLine;
	}

	return finalResult;
}

Compiler::Compiler(std::string path)
{
	std::string content = Files::Read(path.c_str());
	std::istringstream conStream(content);
	std::string getCommand, getLine;
	std::getline(conStream, getCommand);
	std::vector<std::string> getSwitches;
	std::vector<std::pair<std::string, std::string>> getValues;

	content.erase(0, content.find("\n") + 1);
	conStream.str() = content;
	while (std::getline(conStream, getLine))
		getSwitches.push_back(getLine);

	for (int i = 0; i < getSwitches.size(); i++)
	{
		std::istringstream valContent(getSwitches[i]);
		bool isProperty = true;
		while (std::getline(valContent, getLine, '='))
		{
			if (isProperty)
			{
				getValues.push_back(std::make_pair(getLine, ""));
			}
			else
			{
				getValues[getValues.size() - 1].second = getLine;
			}

			isProperty = !isProperty;
		}
	}

	customValues = getValues;
	command = ModifyCommandLine(getCommand);
}

void Compiler::Compile()
{
	std::cout << command << std::endl;
	system(command.c_str());
	std::cout << resultPath.u8string() << std::endl;

	for (auto& files : std::experimental::filesystem::recursive_directory_iterator(resultPath))
	{
		std::string oldFileURL = std::experimental::filesystem::current_path().u8string() + "/" + files.path().filename().u8string();
		std::cout << oldFileURL << " changes from " << files.path().u8string() << std::endl;
		Files::Replace(oldFileURL.c_str(), files.path().u8string().c_str(), true);
	}

	std::string url = std::experimental::filesystem::current_path().u8string() + "/Geometria.exe";
	Files::OpenProgram(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(url).c_str());
	Graphics::Exit();
}

void Compiler::AddImportantFilePath(std::string path)
{
	majorFilePaths.push_back(path);
}

void Compiler::AddCustomValue(std::string name, std::string value)
{
	customValues.push_back(std::make_pair(name, value));
}

void Compiler::AddPropertyChange(std::string name, int file, std::string value)
{
	CompilerValue c;
	c.name = name;
	c.file = file;
	c.value = value;
	compValues.push_back(c);
}

std::string Compiler::ModifyCommandLine(std::string c)
{
	std::string change = c;

	for (int i = 0; i < customValues.size(); i++)
	{
		if (customValues[i].first.find("$_compiler") != std::string::npos)
		{
			customValues[i].second = FillCommandSpaces(customValues[i].second);
		}
		else if (customValues[i].second.find("$_mainDir") != std::string::npos)
		{
			customValues[i].second.replace(customValues[i].second.find("$_mainDir"), sizeof("$_mainDir") - 1, defaultProjectPath.u8string());
		}

		if (customValues[i].first.find("$_resultDir") != std::string::npos)
		{
			std::string finalRD = customValues[i].second;

			while (finalRD.find("\"") != std::string::npos)
				finalRD = finalRD.replace(finalRD.find("\""), sizeof("\"") - 1, "");

			resultPath = finalRD;
		}
	}

	for (int i = 0; i < customValues.size(); i++)
	{
		if (change.find(customValues[i].first) != std::string::npos)
		{
			change.replace(change.find(customValues[i].first), customValues[i].first.size(), customValues[i].second);
		}
	}

	customValues.clear();
	return change;
}
