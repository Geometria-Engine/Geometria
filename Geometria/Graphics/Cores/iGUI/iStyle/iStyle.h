#ifndef I_STYLE_H
#define I_STYLE_H

#include "iFont/iFont.h"
#include <vector>
#include "Tools/Vector.h"

struct iStyle;

struct iStyleManager
{
	static std::vector<iStyle*> allStyles;
	static std::vector<iFont*> allFonts;
};

struct iStyle_Window
{
	bool _resize = true;
	bool _title = true;
	bool _scroll = true;
	bool _move = true;
	bool _setTransformStart = false;

	Vector2 _screenPosition = Vector2(-1, -1);
	Vector2 _screenScale = Vector2(-1, -1);

	bool& IsResizable() { return _resize; }
	bool& ShowTitle() { return _title; }
	bool& CanScroll() { return _scroll; }
	bool& IsDraggable() { return _move; }
	bool& SetTransformAtStart() { return _setTransformStart; }

	Vector2& ScreenPosition() { return _screenPosition; }
	Vector2& ScreenScale() { return _screenScale; }
};

struct iStyle
{
	iFont* font = nullptr;
	int styleId = -1;

	iStyle_Window* _window = nullptr;

	iStyle()
	{
		styleId = iStyleManager::allStyles.size();
		iStyleManager::allStyles.push_back(this);
	}

	iFont*& Font();

	iFont*& Font(std::string file);

	iStyle_Window*& Window();

	void ImGuiBegin()
	{
		if(font != nullptr)
			font->ImGuiBegin();
	}

	void ImGuiEnd()
	{
		if(font != nullptr)
			font->ImGuiEnd();
	}
};

#endif