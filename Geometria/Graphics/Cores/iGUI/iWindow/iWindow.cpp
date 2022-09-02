#include "iWindow.h"

iStyle_Window* iWindow::GetWindowStyle()
{
	if(style != nullptr)
		return style->Window();
	else
		return iGUI::GlobalStyle()->Window();

	return nullptr;
}

void iWindow::UI_SetStyleParams()
{	
	iStyle_Window* getWindowStyle = GetWindowStyle();

	if (!getWindowStyle->IsResizable()) _window_flags |= ImGuiWindowFlags_NoResize;
	if (!getWindowStyle->ShowTitle()) _window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (!getWindowStyle->CanScroll()) _window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!getWindowStyle->IsDraggable()) _window_flags |= ImGuiWindowFlags_NoMove;
}

void iWindow::UI_SetAndGetTransform()
{
	iStyle_Window* getWindowStyle = GetWindowStyle();

	int alignW, alignH;

	if(!getWindowStyle->IsResizable())
		ImGui::SetNextWindowSize(ImVec2(GetTransform().scale.x, GetTransform().scale.y), ImGuiCond_Always);

	if(getWindowStyle->_screenPosition != Vector2(-1, -1))
	{
		alignW = Graphics::GetMainWindow().width * getWindowStyle->_screenPosition.x - (GetTransform().scale.x / (1 / getWindowStyle->_screenPosition.x)) + (_imGuiScreenPos.x / Graphics::GetMainWindow().width) + GetTransform().position.x;
		alignH = Graphics::GetMainWindow().height * getWindowStyle->_screenPosition.y - (GetTransform().scale.y / (1 / getWindowStyle->_screenPosition.y)) + (_imGuiScreenPos.y / Graphics::GetMainWindow().height) - GetTransform().position.y;
	}
	else
	{
		alignW = GetTransform().position.x;
		alignH = GetTransform().position.y;
	}

	if(!getWindowStyle->IsDraggable())
		ImGui::SetNextWindowPos(ImVec2(alignW, alignH), ImGuiCond_Always);
}

void iWindow::OnStart()
{
	iStyle_Window* getWindowStyle = GetWindowStyle();

	if(getWindowStyle->SetTransformAtStart())
	{
		ImGui::SetNextWindowPos(ImVec2(GetTransform().position.x, GetTransform().position.y), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(GetTransform().scale.x, GetTransform().scale.y), ImGuiCond_Always);
	}
}

void iWindow::OnUpdate()
{
	UI_SetStyleParams();

	UI_SetAndGetTransform();

	if(ImGui::Begin(title.c_str(), nullptr, _window_flags))
	{
		_imGuiScreenPos = Vector2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);

		for(auto i : allElements)
			i->OnUpdate();

		ImGui::End();
	}
}