#ifndef FILES_H
#define FILES_H
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

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
	static bool Remove(const char* url);
	static std::vector<unsigned char> GetImageData(const char* url, int& width, int& height);
	static bool SaveImage(std::string output, std::vector<unsigned char> data, int width, int height);
	static bool LoadScene(std::string file);

	static int UnZIP(const char* url);

	static void CreateDirectory(const char* url);
	static bool DirectoryExists(const char* url);
	static std::string GetDirectoryOf(const char* file);
};
#endif