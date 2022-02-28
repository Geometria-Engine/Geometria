#pragma once
#include <vector>

class Input
{
public:
	static bool GetKey(unsigned int keycode);
	static bool GetKeyDown(unsigned int keycode);
	static std::vector<int> checkKeys;

	static void UpdateKeyState();

	static bool GetMouseButton(unsigned int button);
};