#ifndef I_WINDOW_H
#define I_WINDOW_H

#include "../iGUI.h"

struct iWindow : public iGUI
{
	std::string title = "";

	enum Mode
	{
		Window,
		Canvas
	};

	Mode windowMode;

	ImGuiWindowFlags _window_flags = 0;

	Vector2 _imGuiScreenPos;

	iWindow() {}
	iWindow(std::string title)
	{
		this->title = title;
		GetTransform().scale = Vector3(400, 400, 0);
	}

	iWindow(std::string title, Mode mode)
	{
		this->title = title;
		GetTransform().scale = Vector3(400, 400, 0);

		if(mode == Mode::Canvas)
		{
			Style()->Window()->IsResizable() = false;
			Style()->Window()->ShowTitle() = false;
			Style()->Window()->CanScroll() = false;
			Style()->Window()->IsDraggable() = false;
			Style()->Window()->SetTransformAtStart() = true;
		}
	}

	iStyle_Window* GetWindowStyle();

	void UI_SetStyleParams();
	void UI_SetAndGetTransform();
	void OnStart();
	void OnUpdate();
};

#endif