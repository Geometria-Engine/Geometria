#include "iGUI.h"

iStyle* iGUI::_globalStyle = nullptr;
bool iGUI::_globalBegin = true;

void iGUI::OnStartup()
{
	ClassType = Class::Pointer;
}

void iGUI::SetGlobalStyle()
{
	if(_globalStyle == nullptr)
	{
		iGUI::_globalStyle = new iStyle();

		iGUI::_globalStyle->font = new iFont();
		iGUI::_globalStyle->font->file = "EngineResources/Fonts/Raleway-Regular.ttf";
		iGUI::_globalStyle->font->size = 20;

		iGUI::_globalStyle->_color = Color::white();
		iGUI::_globalStyle->_backgroundColor = Color(0, 0, 0, 0.5);

		iGUI::_globalStyle->Window();
		iGUI::_globalStyle->Window()->Border()->MainColor() = Color(0.5, 0.5, 0.5, 1);
		iGUI::_globalStyle->Window()->Border()->Size() = 1;
	}
}

iStyle* iGUI::GlobalStyle()
{
	if(_globalStyle == nullptr)
		SetGlobalStyle();

	return _globalStyle;
}

iStyle* iGUI::Style()
{
	if(style == nullptr)
	{
		style = new iStyle();
	}

	return style;
}

iStyle* iGUI::CurrentStyle()
{
	if(style != nullptr)
		return style;

	return iGUI::GlobalStyle();
}

void iGUI::GlobalFrameBegin()
{
	SetGlobalStyle();

	for(auto i : iStyleManager::allStyles)
	{
		i->ImGuiBegin();
	}
}

void iGUI::GlobalFrameEnd()
{
	for(auto i : iStyleManager::allStyles)
	{
		i->ImGuiEnd();
	}
}