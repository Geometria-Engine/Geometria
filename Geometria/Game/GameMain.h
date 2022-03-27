#pragma once

#include "geometria.h"

struct GameMain
{
	static void Init()
	{
		std::vector<float> f = Files::ReadOBJ("Game/Veyron.obj");
		Model* model = new Model(f, Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1, 1, 1));
		RendererCore::AddModel(*model);
		
		std::cout << "Hello World!" << std::endl;
	}
};