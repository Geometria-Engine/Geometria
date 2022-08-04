#ifndef FILES_H
#define FILES_H
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "Trenchbroom/Trenchbroom.h"

#ifdef _WIN32
#include <windows.h>
#undef CreateDirectory
#endif

extern "C"
{
	#include "Files/ZIP/unzip.h"
}

struct Files
{
	static std::string Read(const char* url);
	static std::string Read(const char* url, bool isBinary);
	static std::vector<std::string> ReadAndGetLines(const char* url);
	static std::string Write(const char* url, std::string content);
	static std::string Replace(const char* oldFile, const char* newFile, bool isBinary);
	static std::string Replace(const char* oldFile, const char* newFile);
	static std::string OpenImage(const char* url, int& width, int& height);

	static std::vector<std::string> OpenTexturePack(const char* gtxp);

	static void OpenProgram(const wchar_t* url);
	static void OpenProgram(const char* url);

	static bool Remove(const char* url);
	static std::vector<unsigned char> GetImageData(const char* url, int& width, int& height);
	static bool SaveImage(std::string output, std::vector<unsigned char> data, int width, int height);
	static bool LoadScene(std::string file);

	static std::vector<float> ReadOBJ(const char* url);

	static int UnZIP(const char* url);

	static void CreateDirectory(const char* url);
	static void CopyDirectory(const char* url, const char* dest);
	static void DeleteDirectory(const char* url);
	static bool DirectoryExists(const char* url);
	static std::string GetDirectoryOf(const char* file);
	static std::string GetFilenameFromDirectory(const char* dir);

	static std::string GetGamePath();

	static std::string WhereIs(std::string name);

	static std::string RunCommand(std::string cmd);
	static std::string GetPathFromCommand(std::string cmd);
	static std::string GetValueFromCommand(std::string cmd);

	static void ClearConsole();
	static void ShowConsole()
	{
		#ifdef _WIN32
			ShowWindow(GetConsoleWindow(), SW_SHOW);
		#endif
	}
	static void HideConsole()
	{
		#ifdef _WIN32
			ShowWindow(GetConsoleWindow(), SW_HIDE);
		#endif
	}

	static void ChangeCurrentDirectory(std::string path);

	static std::string ConvertToWindowsCmdPath(std::string path);

	static std::string GetExecutablePath();
};
#endif