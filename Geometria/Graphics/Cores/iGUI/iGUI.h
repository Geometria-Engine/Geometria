#ifndef IGUI_MANAGER_H
#define IGUI_MANAGER_H

#include "geometria.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "ImGUI/imgui_internal.h"

#include <string>

#include "iStyle/iStyle.h"

struct iGUI : public ScriptBehaviour
{
	int ownerSceneId;
	int ownerDrawId;

	iGUI* owner = nullptr;

	iStyle* style = nullptr;

	iStyle* Style();
	iStyle* CurrentStyle();

	static iStyle* _globalStyle;
	static bool _globalBegin;

	std::vector<iGUI*> allElements;

	static void SetGlobalStyle();

	static iStyle* GlobalStyle();

	void OnStartup();

	static void GlobalFrameBegin();
	static void GlobalFrameEnd();
};

#endif