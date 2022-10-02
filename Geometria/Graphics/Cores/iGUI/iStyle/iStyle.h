#ifndef I_STYLE_H
#define I_STYLE_H

#include "iFont/iFont.h"
#include <vector>
#include "Tools/Vector.h"
#include "Tools/Color.h"
#include "Graphics/Cores/Texture/Texture.h"

struct iStyle;

struct iStyleManager
{
	static std::vector<iStyle*> allStyles;
	static std::vector<iFont*> allFonts;
};

struct iStyle_Border
{
	Color _borderColor = Color(-1, -1, -1, -1);
	int _borderSize = 0;
	int _radiusSize = 0;

	Color& MainColor()
	{
		return _borderColor;
	}

	int& Size()
	{
		return _borderSize;
	}

	int& Radius()
	{
		return _radiusSize;
	}
};

struct iGradient
{
	std::vector<std::pair<Color, float>> gradients;

	std::pair<Color, float> Add(Color col, float f)
	{
		gradients.push_back(std::make_pair(col, f));
		return gradients.back();
	}

	void Clear()
	{
		gradients.clear();
		std::vector<std::pair<Color, float>>().swap(gradients);
	}
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

	iStyle_Border* border = nullptr;

	bool& IsResizable() { return _resize; }
	bool& ShowTitle() { return _title; }
	bool& CanScroll() { return _scroll; }
	bool& IsDraggable() { return _move; }
	bool& SetTransformAtStart() { return _setTransformStart; }

	Vector2& ScreenPosition() { return _screenPosition; }
	Vector2& ScreenScale() { return _screenScale; }

	iStyle_Border*& Border();
	iStyle_Border*& CurrentBorder();
};

struct iStyle
{
	iFont* font = nullptr;
	int styleId = -1;

	iStyle_Window* _window = nullptr;

	Color _color = Color(-1, -1, -1, -1);
	Color _backgroundColor = Color(-1, -1, -1, -1);

	iGradient* _backgroundGradient = nullptr;

	Texture* _backgroundImage = nullptr;

	iStyle()
	{
		styleId = iStyleManager::allStyles.size();
		iStyleManager::allStyles.push_back(this);
	}

	iFont*& Font();

	iFont*& Font(std::string file);

	iStyle_Window*& Window();

	Color& MainColor()
	{
		return _color;
	}

	Color& BackgroundColor()
	{
		return _backgroundColor;
	}

	iGradient*& BackgroundGradient();

	Texture*& BackgroundImage()
	{
		return _backgroundImage;
	}

	void UI_PushColors();
	void UI_PopColors();

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