#include <GL/glew.h>
#include <glfw3.h>

#include "MaxReactsBinPack/MaxRectsBinPack.h"
#ifndef TEXTURE_H
#define TEXTURE_H
class Texture
{
public:

	int width = 0;
	int	height = 0;
	int type = 0;
	std::string filename;
	std::vector<unsigned char> data;
	rbp::Rect finalRect;
	int texGroupId = 0;
	bool isLoadedToGPU = false;

	enum Type
	{
		Default,
		Depth
	};

	Texture();
	Texture(const char* fileName, Type type);
	Texture(std::string fileName, Type type);
	bool IsLoadedToGPU() { return isLoadedToGPU; }
	~Texture() {};
};

class TextureGroup
{
public:
	std::vector<Texture*> allTextures;
	int width = 0;
	int	height = 0;
	std::vector<unsigned char> groupData;
	int id = 0;

	rbp::MaxRectsBinPack bin;

	//==[ OPENGL ]==//
	GLuint texture = 0;
	GLenum type = 0;
	GLint textureUnit = 0;
	//==[ OPENGL ]==//

	enum Type
	{
		Default,
		Depth
	};

	TextureGroup() {};
	void AddTexture(Texture& tex);
	void UploadToGPU();
	~TextureGroup();

	inline GLuint getID() const;

	void Enable(const GLint tUnit);
	void Disable();
};

class TextureManager
{
public:
	static std::vector<TextureGroup> textureGroups;
	static std::vector<Texture*> OpenTexturePack(const char* url);
	//static std::vector<Texture*> OpenSpriteSheet(const char* url);

	static void ClearRAM()
	{
		for (int i = 0; i < textureGroups.size(); i++)
		{
			for (auto t : textureGroups[i].allTextures)
			{
				if (t != nullptr)
					delete t;

				t = nullptr;
			}
			
			textureGroups[i].allTextures.clear();
		}

		textureGroups.clear();
	}

	static void UploadToGPU()
	{
		for (int i = 0; i < textureGroups.size(); i++)
		{
			textureGroups[i].UploadToGPU();
		}
	}

	static void EnableTextures()
	{
		for (int i = 0; i < textureGroups.size(); i++)
		{
			textureGroups[i].Enable(i);
		}
	}

	static void DisableTextures()
	{
		for (int i = 0; i < textureGroups.size(); i++)
		{
			textureGroups[i].Disable();
		}
	}
};
#endif
