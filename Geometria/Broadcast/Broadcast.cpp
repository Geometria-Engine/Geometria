#include "Broadcast.h"
#include <String\StringAPI.h>

std::vector<std::pair<ImGUIElement*, std::string>> Broadcast::imguibroadcaster;
std::vector<std::pair<std::string, int>> Broadcast::messages;

bool Broadcast::Recieve(std::string message)
{
	for (int i = 0; i < messages.size(); i++)
	{
		std::string messageFinal = messages[i].first;
		for (int g = 0; g < imguibroadcaster.size(); g++)
		{
			if (imguibroadcaster[g].first == nullptr)
				imguibroadcaster.erase(imguibroadcaster.begin() + g);
			else if(imguibroadcaster[g].first->guiType == ImGUIElement::GUIType::InputField)
				messageFinal = StringAPI::ReplaceAll(messageFinal, "{" + imguibroadcaster[g].second + "}", imguibroadcaster[g].first->strData);
		}

		if (messageFinal == message)
		{
			messages[i].second++;
			return true;
		}
	}

	return false;
}

void Broadcast::Send(std::string message) { messages.push_back(std::make_pair(message, 0)); }

void Broadcast::Update()
{
	for(int i = 0; i < messages.size(); i++)
		if(messages[i].second > 2)
			messages.erase(messages.begin() + i);
}

void Broadcast::AddImGUIElementToBroadcast(ImGUIElement* i, std::string s)
{
	std::cout << "=================> BROADCAST INPUT FIELD: " << s << std::endl;
	imguibroadcaster.push_back(std::make_pair(i, s));
}

std::string Broadcast::ShowBroadcastMessage(std::string s)
{
	for (int g = 0; g < imguibroadcaster.size(); g++)
	{
		if (imguibroadcaster[g].first == nullptr)
			imguibroadcaster.erase(imguibroadcaster.begin() + g);
		else if (imguibroadcaster[g].first->guiType == ImGUIElement::GUIType::InputField)
			s = StringAPI::ReplaceAll(s, "{" + imguibroadcaster[g].second + "}", imguibroadcaster[g].first->strData);
	}

	return s;
}
