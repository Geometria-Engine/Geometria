#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H

#include "String/StringAPI.h"

class Application
{
public:

	enum State
	{
		Editor,
		Game
	};

	enum Platform
	{
		Windows,
		Linux
	};

	static std::string GetLinuxDistro();

	static State _engineState;

	static bool IsPlatform(Platform p);

	static void SetEditor();
	static void SetGame();
	static void Start();
};
#endif
