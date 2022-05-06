#ifndef MINI_SHADER_H
#define MINI_SHADER_H

#include <iostream>
#include <string>
#include <vector>

struct MiniShader
{
	std::string vertUrl, fragUrl;
	std::string vertContent, fragContent;
	int miniShaderId;

	MiniShader()
	{
	}

	MiniShader(std::string fragmentUrl, std::string vertexUrl);

	static std::string RewriteForShaderVersion(std::string content, float ver, bool isFragmentShader);
};

struct MiniShaderManager
{
	static std::vector<MiniShader*> allMiniShaders;
};

#endif