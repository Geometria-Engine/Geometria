#include "iStyle.h"
#include "Graphics/Cores/iGUI/iGUI.h"

std::vector<iStyle*> iStyleManager::allStyles;
std::vector<iFont*> iStyleManager::allFonts;

iFont*& iStyle::Font()
{
	return Font("");
}

iFont*& iStyle::Font(std::string file)
{
	if(font == nullptr)
	{
		font = new iFont();

		if(file == "")
		{
			if(iGUI::GlobalStyle() != nullptr)
				font->file = iGUI::GlobalStyle()->font->file;
			else
				font->file = "EngineResources/Fonts/Raleway-Regular.ttf";
		}
		else
			font->file = file;

		font->size = 20;
	}

	return font;
}

iStyle_Window*& iStyle::Window()
{
	if(_window == nullptr)
		_window = new iStyle_Window();

	return _window;
}