#include "iStyle.h"
#include "Graphics/Cores/iGUI/iGUI.h"

std::vector<iStyle*> iStyleManager::allStyles;
std::vector<iFont*> iStyleManager::allFonts;

iStyle_Border*& iStyle_Window::Border()
{
	if(border == nullptr)
		border = new iStyle_Border();

	return border;
}

iStyle_Border*& iStyle_Window::CurrentBorder()
{
	if(border != nullptr)
		return border;

	return iGUI::GlobalStyle()->Window()->Border();
}

Vector2& iStyle::ScreenPosition() { return _screenPosition; }
Vector2& iStyle::ScreenScale() { return ScreenScale(true); }

Vector2& iStyle::ScreenScale(bool t)
{
	_setScaleToZero = t;
	return _screenScale;
}

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

iGradient*& iStyle::BackgroundGradient()
{
	if(_backgroundGradient == nullptr)
		_backgroundGradient = new iGradient();

	return _backgroundGradient;
}

iStyle_Window*& iStyle::Window()
{
	if(_window == nullptr)
		_window = new iStyle_Window();

	return _window;
}

void iStyle::UI_PushColors()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	iStyle* globalStyle = iGUI::GlobalStyle();

	style->Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);

	if(_color != Color(-1, -1, -1, -1))
		style->Colors[ImGuiCol_Text] = ImVec4(_color.r, _color.g, _color.b, _color.a);
	else
		style->Colors[ImGuiCol_Text] = ImVec4(globalStyle->_color.r, globalStyle->_color.g, globalStyle->_color.b, globalStyle->_color.a);

	if(_backgroundColor != Color(-1, -1, -1, -1))
		style->Colors[ImGuiCol_WindowBg] = ImVec4(_backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
	else
		style->Colors[ImGuiCol_WindowBg] = ImVec4(globalStyle->_backgroundColor.r, globalStyle->_backgroundColor.g, globalStyle->_backgroundColor.b, globalStyle->_backgroundColor.a);
}

void iStyle::UI_PopColors()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	iStyle* globalStyle = iGUI::GlobalStyle();

	if(_color != Color(-1, -1, -1, -1))
		style->Colors[ImGuiCol_Text] = ImVec4(globalStyle->_color.r, globalStyle->_color.g, globalStyle->_color.b, globalStyle->_color.a);

	if(_backgroundColor != Color(-1, -1, -1, -1))
		style->Colors[ImGuiCol_WindowBg] = ImVec4(globalStyle->_backgroundColor.r, globalStyle->_backgroundColor.g, globalStyle->_backgroundColor.b, globalStyle->_backgroundColor.a);
}