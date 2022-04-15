#ifndef TRENCHBROOM_H
#define TRENCHBROOM_H

#include "Files/Files.h";
#include "Tools/Vector.h"
#include <vector>
#include <array>
#include "String/StringAPI.h"
#include <iostream>
#include "Graphics/Externals/Model.h"
#include "Tools/Math.h"
#include "Tools/Plane.h"
#include "Tools/Face.h"

struct QuakeFaceDictionary
{
	static void Order(std::vector<std::array<float, 4>>& quake, std::vector<std::array<float, 4>>& res, int amount);
};

struct Brush
{
	std::vector<Face> faces;
};

struct Trenchbroom
{
	std::vector<Brush> brushes;
	void LoadMAP(std::string url);

	bool WorldSpawnFound(std::string line)
	{
		return line.find("worldspawn") != std::string::npos;
	}
	
	bool OpenBracketFound(std::string line)
	{
		return line.find("{") != std::string::npos;
	}
	
	bool CloseBracketFound(std::string line)
	{
		return line.find("}") != std::string::npos;
	}
	
	bool BrushInfoFound(std::string line)
	{
		return line.find("(") != std::string::npos;
	}

	Model* MAPToModel();
};

#endif