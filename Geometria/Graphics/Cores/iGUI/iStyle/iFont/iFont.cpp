#include "iFont.h"
#include "Graphics/Cores/iGUI/iGUI.h"
#include "Graphics/Cores/iGUI/iStyle/iStyle.h"

ImFont* iFont::defaultFont = nullptr;

void iFont::ImGuiBegin()
{
	ImGuiIO& io = ImGui::GetIO();

	if(defaultFont == nullptr)
		defaultFont = io.Fonts->AddFontDefault();

	if(file != _lastFile)
	{
		for(auto i : iStyleManager::allFonts)
		{
			if(i->file == file)
			{
				font = i->font;
				break;
			}
		}

		ImFontConfig config;
		config.FileName = file;

		if(font == nullptr)
		{
			font = io.Fonts->AddFontFromFileTTF(file.c_str(), 100, &config);
			iStyleManager::allFonts.push_back(this);
		}

		_lastFile = file;
	}

	//ImGui::PushFont(font);
}