#pragma once

#ifndef APPLICATION_H
#define APPLICATION_H
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

	static State _engineState;

	static bool IsPlatform(Platform p);

	static void SetEditor();
	static void SetGame();
	static void Start();
};
#endif