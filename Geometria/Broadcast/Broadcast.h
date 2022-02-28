#pragma once

#include "geometria.h"
#ifndef BROADCAST_H
#define BROADCAST_H
struct Broadcast
{
	static std::vector<std::pair<ImGUIElement*, std::string>> imguibroadcaster;
	static std::vector<std::pair<std::string, int>> messages;

	static bool Recieve(std::string message);
	static void Send(std::string message);
	static void Update();
	static void AddImGUIElementToBroadcast(ImGUIElement* i, std::string s);
	static std::string ShowBroadcastMessage(std::string s);
};
#endif