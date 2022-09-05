#ifndef I_IMGUI_DEMO_H
#define I_IMGUI_DEMO_H

#include "../iGUI.h"

struct iImGuiDemo : public iGUI
{
	iImGuiDemo()
	{
		
	}

	iImGuiDemo(iGUI& owner)
	{
		owner.allElements.push_back(this);
	}

	void OnUpdate()
	{
		//CurrentStyle()->UI_PushColors();

		ImGui::ShowDemoWindow();

		//CurrentStyle()->UI_PopColors();
	}
};

#endif