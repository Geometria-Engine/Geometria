#include "MiniShader.h"
#include "String/StringAPI.h"
#include "Files/Files.h"

std::vector<MiniShader*> MiniShaderManager::allMiniShaders;

MiniShader::MiniShader(std::string fragmentUrl, std::string vertexUrl)
{
	std::cout << "Mini Shader Created!" << std::endl;

	fragUrl = fragmentUrl;
	vertUrl = vertexUrl;

	if(fragUrl != "")
		fragContent = Files::Read(fragUrl.c_str());

	if(vertUrl != "")
		vertContent = Files::Read(vertUrl.c_str());

	
	miniShaderId = MiniShaderManager::allMiniShaders.size();
	MiniShaderManager::allMiniShaders.push_back(this);
}

std::string MiniShader::RewriteForShaderVersion(std::string content, float ver, bool isFragmentShader)
{
	std::cout << "Rendering shader with version: " << ver << std::endl;
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

	if(content.find("{ApplyMiniShaders}") != std::string::npos)
	{
		std::string miniS;
		miniS += "switch(Master_MiniShaderID)\n";
		miniS += "{\n";
		for(auto i : MiniShaderManager::allMiniShaders)
		{
			miniS += "case ";
			miniS += std::to_string(i->miniShaderId);
			miniS += ":\n";

			if(isFragmentShader)
				miniS += i->fragContent;
			else
				miniS += i->vertContent;

			miniS += "\n";
			miniS += "break;\n";
		}
		miniS += "}\n";

		content = StringAPI::ReplaceAll(content, "{ApplyMiniShaders}", miniS);

		//std::cout << content << std::endl;
	}

	return content;
}