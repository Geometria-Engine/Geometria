#pragma once
#ifndef DRAG_BORDERLESS_WINDOW_H
#define DRAG_BORDERLESS_WINDOW_H

#include "geometria.h"

struct DragBorderlessWindow : public ScriptBehaviour
{
	Vector2 mouseClick;
	bool is_clicked;
	void OnStartup()
	{
		isUniversal = true;
	}

	void OnStart();
	void OnUpdate();
};

#endif