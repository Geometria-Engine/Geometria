#ifndef MINI_SHADER_H
#define MINI_SHADER_H

#include <iostream>
#include <string>
#include <vector>
#include "String/StringAPI.h"

struct MiniShader
{
	std::string vertUrl, fragUrl;
	std::string vertContent, fragContent;

	static std::string RewriteForShaderVersion(std::string content, float ver, bool isFragmentShader)
	{
		if(ver == 1.20f)
		{
			std::cout << "Converting Shader to OpenGL 2.1...\n";

			std::string ver = "#version 120\n";
			std::string res = ver + content;
			content = res;

			if(!isFragmentShader)
			{
				content = StringAPI::ReplaceAll(content, "in ", "attribute ");
				content = StringAPI::ReplaceAll(content, "out ", "varying ");
				content = StringAPI::ReplaceAll(content, "flat out float ", "flat varying float ");
			}
			else
			{
				content = StringAPI::ReplaceAll(content, "in ", "varying ");
				content = StringAPI::ReplaceAll(content, "flat in float ", "flat varying float ");

				content = StringAPI::RemoveAll(content, "out vec4 fs_color;");
				content = StringAPI::ReplaceAll(content, "fs_color", "gl_FragColor");
			}
		}
		else if(ver == 1.30f)
		{
			std::string ver = "#version 130\n";
			std::string res = ver + content;
			content = res;
		}
		else if(ver < 1.20f)
		{
			std::cout << "[ERROR]: Only versions that are OpenGL 2.1 and below are supported." << std::endl;
		}

		return content;
	}
};

#endif