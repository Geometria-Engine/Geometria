#include "ImGUIElement.h"
#include "../Cores/Renderer/RendererCore.h"
#include "../Externals/SceneAndDrawCall.h"
#include <experimental/filesystem>
#include "../Cores/MainAPI/Graphics.h"
#include "../../Input/Input.h"
#include "Broadcast/Broadcast.h"

bool ImGUIElement::_isMouseOnAnyWindow = false;
std::vector<ImGUIEFont*> ImGUIElement::allFonts;

void ImGUIElement::LoadElements()
{
	
}

ImGUIEFont* ImGUIElement::AddFont(const char* name, const char* url, int size)
{
	ImGUIEFont* font = new ImGUIEFont(name, url, size);
	allFonts.push_back(font);
	return font;
}

ImGUIEFont* ImGUIElement::GetFont(const char* nameOrUrl, int size)
{
	bool isUrl = false;
	std::string nu = nameOrUrl;
	if (nu.find("/") != std::string::npos)
		isUrl = true;

	for (auto i : allFonts)
	{
		if (isUrl)
		{
			if (i->size == size && i->url == nu)
				return i;
		}
		else
		{
			if (i->size == size && i->name == nu)
				return i;
		}
	}

	return nullptr;
}

void ImGUIElement::Move(Vector2 position)
{
	moveToPosition = position;
	_requestForceMove = true;
}

void ImGUIElement::Scale(Vector2 scale)
{
	scaleTo = scale;
	size = scale;
	_requestForceScale = true;
}

void ImGUIElement::ScaleWithScreenResolution(Vector2 percentageScale)
{
	int finalW = 0, finalH = 0;

	if (screenSize.x == -1)
		finalW = size.x;
	else
	{
		if (owner == nullptr)
			finalW = Graphics::GetMainWindow().width * (screenSize.x / 100.0f);
		else
			finalW = owner->size.x * (screenSize.x / 100.0f);
	}

	if (minScale.x != -1)
		if (finalW < minScale.x)
			finalW = minScale.x;

	if (maxScale.x != -1)
		if (finalW > maxScale.x)
			finalW = maxScale.x;


	if (screenSize.y == -1)
	{
		finalH = size.y;
	}
	else
	{
		if (owner == nullptr)
			finalH = Graphics::GetMainWindow().height * (screenSize.y / 100.0f);
		else
			finalH = owner->size.y * (screenSize.y / 100.0f);
	}

	if (minScale.y != -1)
		if (finalH < minScale.y)
			finalH = minScale.y;

	if (maxScale.y != -1)
		if (finalH > maxScale.y)
			finalH = maxScale.y;


	Scale(Vector2(finalW, finalH));
}

void ImGUIElement::SetCurrentSizeToScreenSize(bool setX, bool setY)
{
	if (setX)
	{
		if(owner == nullptr)
			screenSize.x = (size.x / Graphics::GetMainWindow().width) * 100.0f;
		else
			screenSize.x = (size.x / owner->size.x) * 100.0f;
	}

	if (setY)
	{
		if (owner == nullptr)
			screenSize.y = (size.y / Graphics::GetMainWindow().height) * 100.0f;
		else
			screenSize.y = (size.y / owner->size.y) * 100.0f;
	}
}

void ImGUIElement::SetCurrentPosToAlignPivot()
{
	if (alignPivot != Vector2(-1, -1))
	{
		ForceAlignToCustom();

		if (guiType == Window)
		{
			alignPivot = Vector2((ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2.0f)) / Graphics::GetMainWindow().width,
				(ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y / 2.0f)) / Graphics::GetMainWindow().height);
		}
	}
}

bool ImGUIElement::IsMouseOnAnyWindow()
{
	return _isMouseOnAnyWindow;
}

void ImGUIElement::SetAlignPivot(Vector2 a)
{
	Alignment = AlignTo::Custom;
	alignPivot = a;
}

void ImGUIElement::ForceAlignToCustom()
{
	Alignment = AlignTo::Custom;
}

void ImGUIElement::OpenWithMouseButton(int input)
{
	mouseInput = input;
}

void ImGUIElement::SetColor(Color col)
{
	isColored = true;
	colorRef = new Color(col);
}

void ImGUIElement::SetFontColor(Color col)
{
	isColored = true;

	if (IsColorRefUsedForBackground())
		fontColor = col;
	else
		colorRef = new Color(col);
}

void ImGUIElement::SetMinScale(Vector2 min)
{
	minScale = min;
}

void ImGUIElement::SetMaxScale(Vector2 max)
{
	maxScale = max;
}

void ImGUIElement::AddBackgroundGradient(std::vector<std::pair<Color, float>> bg)
{
	RemoveBackgroundGradient();
	backgroundGradients = bg;
}

void ImGUIElement::RemoveBackgroundGradient()
{
	backgroundGradients.clear();
	std::vector<std::pair<Color, float>>().swap(backgroundGradients);
}

void ShadeVertsLinearColorGradientSetAlpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1)
{
    ImVec2 gradient_extent = ImVec2(gradient_p1.x - gradient_p0.x, gradient_p1.y - gradient_p0.y);
    float gradient_inv_length2 = 1.0f / ImLengthSqr(gradient_extent);
    ImDrawVert* vert_start = draw_list->VtxBuffer.Data + vert_start_idx;
    ImDrawVert* vert_end = draw_list->VtxBuffer.Data + vert_end_idx;
    const int col0_r = (int)(col0 >> IM_COL32_R_SHIFT) & 0xFF;
    const int col0_g = (int)(col0 >> IM_COL32_G_SHIFT) & 0xFF;
    const int col0_b = (int)(col0 >> IM_COL32_B_SHIFT) & 0xFF;
    const int col0_a = (int)(col0 >> IM_COL32_A_SHIFT) & 0xFF;
    const int col_delta_r = ((int)(col1 >> IM_COL32_R_SHIFT) & 0xFF) - col0_r;
    const int col_delta_g = ((int)(col1 >> IM_COL32_G_SHIFT) & 0xFF) - col0_g;
    const int col_delta_b = ((int)(col1 >> IM_COL32_B_SHIFT) & 0xFF) - col0_b;
    const int col_delta_a = ((int)(col1 >> IM_COL32_A_SHIFT) & 0xFF) - col0_a;
    for (ImDrawVert* vert = vert_start; vert < vert_end; vert++)
    {
        float d = ImDot(ImVec2(vert->pos.x - gradient_p0.x, vert->pos.y - gradient_p0.y), gradient_extent);
        float t = ImClamp(d * gradient_inv_length2, 0.0f, 1.0f);
        int r = (int)(col0_r + col_delta_r * t);
        int g = (int)(col0_g + col_delta_g * t);
        int b = (int)(col0_b + col_delta_b * t);
        int a = (int)(col0_a + col_delta_a * t);
        vert->col = (r << IM_COL32_R_SHIFT) | (g << IM_COL32_G_SHIFT) | (b << IM_COL32_B_SHIFT) | (a << IM_COL32_A_SHIFT);
    }
}

void ImGUIElement::RenderGradients(ImDrawList* drawList, std::vector<std::pair<Color, float>>& b, ImVec2 topL, ImVec2 botR)
{
	std::vector<std::pair<Color, float>> finalB;

	if (b[0].second != 0)
		finalB.push_back(std::make_pair(b[0].first, 0));

	for (auto i : b)
		finalB.push_back(i);

	if(b[b.size() - 1].second != 100)
		finalB.push_back(std::make_pair(b[b.size() - 1].first, 100));

	for (int i = 1; i < finalB.size(); i++)
	{
		ImVec2 topLFinal = topL, botRFinal = botR;

		topLFinal.y = (ImGui::GetWindowSize().y * finalB[i - 1].second / 100) + topL.y;
		botRFinal.y = (ImGui::GetWindowSize().y * finalB[i].second / 100) + topL.y;

		/*std::cout << "Top Left: " << topLFinal.x << " x " << topLFinal.y << " || Percentage: " << finalB[i - 1].second << '\n';
		std::cout << "Bottom Right: " << botRFinal.x << " x " << botRFinal.y << " || Percentage: " << finalB[i].second << '\n';
		std::cout << "--------------------------------" << '\n';*/

		ImColor col2(finalB[i].first.r, finalB[i].first.g, finalB[i].first.b, finalB[i].first.a);
		ImColor col1(finalB[i - 1].first.r, finalB[i - 1].first.g, finalB[i - 1].first.b, finalB[i - 1].first.a);

		/*ImGui::GetWindowDrawList()->AddRectFilledMultiColor(topLFinal, botRFinal,
			col1, col1, col2, col2);*/

		float finalBorderRadius = 0;
		ImDrawFlags flags;
		if (i == 1)
		{
			if (finalB.size() == 2)
				flags = ImDrawCornerFlags_All;
			else
				flags = ImDrawCornerFlags_Top;

			finalBorderRadius = borderRadius;
		}
		else if (i == finalB.size() - 1)
		{
			flags = ImDrawCornerFlags_Bot;
			finalBorderRadius = borderRadius;
		}

		ImDrawList* final_drawList = drawList;

		if (final_drawList == nullptr)
			final_drawList = ImGui::GetWindowDrawList();

		const int vtx_idx_0 = final_drawList->VtxBuffer.Size;
		final_drawList->AddRectFilled(topLFinal, botRFinal, ImColor(1, 1, 1, 1), finalBorderRadius, flags);
		const int vtx_idx_1 = final_drawList->VtxBuffer.Size;
		final_drawList->AddRectFilled(topLFinal, botRFinal, ImColor(1, 1, 1, 1), finalBorderRadius, flags);
		const int vtx_idx_2 = final_drawList->VtxBuffer.Size;

		ShadeVertsLinearColorGradientSetAlpha(final_drawList, vtx_idx_0, vtx_idx_1, topLFinal, ImVec2(botRFinal.x, topLFinal.y), col1, col2);
		ShadeVertsLinearColorGradientSetAlpha(final_drawList, vtx_idx_1, vtx_idx_2, topLFinal, ImVec2(topLFinal.x, botRFinal.y), col1, col2);
	}
}

void ImGUIElement::RenderBoxShadow()
{
	Vector2 lastPosPlusOffset = lastPosition + boxShadow->offset;
	Vector2 lastPosPlusSize = lastPosPlusOffset + lastSize;

	ImDrawList* draw_list = nullptr;
	if (owner != nullptr)
		draw_list = ImGui::GetWindowDrawList();
	else
		draw_list = ImGui::GetBackgroundDrawList();

	draw_list->AddShadowRect(ImVec2(lastPosPlusOffset.x, lastPosPlusOffset.y),
		ImVec2(lastPosPlusSize.x, lastPosPlusSize.y),
		ImColor(boxShadow->col.r, boxShadow->col.g, boxShadow->col.b, boxShadow->col.a),
		boxShadow->blur, ImVec2(0, 0),
		ImDrawFlags_None, borderRadius);
}

ImGUIEBoxShadow* ImGUIElement::AddBoxShadow(Color shadowCol, Vector2 offset)
{
	if (boxShadow == nullptr)
		boxShadow = new ImGUIEBoxShadow();

	boxShadow->col = shadowCol;
	boxShadow->offset = offset;

	return boxShadow;
}

ImGUIEBoxShadow* ImGUIElement::AddBoxShadow(Color shadowCol, Vector2 offset, float blur)
{
	if (boxShadow == nullptr)
		boxShadow = new ImGUIEBoxShadow();

	boxShadow->col = shadowCol;
	boxShadow->offset = offset;
	boxShadow->blur = blur;

	return boxShadow;
}

ImGUIEBoxShadow* ImGUIElement::RemoveBoxShadow()
{
	if (boxShadow != nullptr)
	{
		delete boxShadow;
		boxShadow = nullptr;
	}
	return nullptr;
}

void ImGUIElement::RunBroadcastAnimations()
{
	for (auto i : broadcastAnimations)
	{
		if (Broadcast::Recieve(i.second))
		{
			i.first->Play();
		}
	}
}

void ImGUIElement::BroadcastOnClick(std::string message)
{
	if (bcastOnClickIndex != -1)
		onClickEvents.erase(onClickEvents.begin() + bcastOnClickIndex);

	bcastOnClickIndex = onClickEvents.size();
	onClickEvents.push_back(std::bind(Broadcast::Send, message));
}

void ImGUIElement::Delete()
{
	onClickEvents.clear();
	onChangeEvents.clear();
	onFileChangeEvents.clear();
	onUIUpdateBeginEvents.clear();
	onUIUpdateEndEvents.clear();

	for (int i = 0; i < allElements.size(); i++)
	{
		allElements[i]->Delete();

		for (int h = 0; h < Hierarchy::allScripts.size();)
		{
			if (Hierarchy::allScripts[h]->scriptId == allElements[i]->scriptId)
			{
				Hierarchy::allScripts.erase(Hierarchy::allScripts.begin() + h);
			}
			else
			{
				h++;
			}
		}
	}
	//std::vector<ScriptBehaviour*>().swap(Hierarchy::allScripts);
	allElements.clear();
}

void ImGUIElement::OnBegin()
{

}

ImGUIElement::ImGUIElement() : guiType(GUIType::None) {}
ImGUIElement::ImGUIElement(GUIType gT) : guiType(gT) {}
ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner) : guiType(gT) 
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}
ImGUIElement::ImGUIElement(GUIType gT, std::string t) : guiType(gT), text(t) {}
ImGUIElement::ImGUIElement(GUIType gT, int value) : guiType(gT), val(value) {}
ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, int value) : guiType(gT), val(value)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}
ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, float* floatR) : guiType(gT), fRef(floatR)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}
ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, float* floatR, float min, float max) : guiType(gT), fRef(floatR), minVal(min), maxVal(max)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}
ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t) : guiType(gT), text(t) 
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, int* iR) : guiType(gT), text(t), iRef(iR)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, std::string t2) : guiType(gT), text(t), secondText(t2)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, Vector4* v4r) : guiType(gT), text(t), v4ref(v4r) 
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, std::string* targ) : guiType(gT), text(t), target(targ)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, std::string t, Color* colRef) : guiType(gT), text(t), colorRef(colRef)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

ImGUIElement::ImGUIElement(GUIType gT, ImGUIElement& owner, ScriptBehaviour* sr) : guiType(gT), scriptRef(sr)
{
	owner.allElements.push_back(this);
	this->owner = &owner;
}

float vectorToFloat[3] = { 0, 0, 0 };
void ImGUIElement::OnUpdate()
{
	if (isEnabled)
	{
		if (mouseInput != -1)
		{
			if (Input::GetMouseButton(mouseInput))
			{
				isOpen = true;
			}
		}

		for (int i = 0; i < onUIUpdateBeginEvents.size(); i++)
		{
			onUIUpdateBeginEvents[i]();
		}


		bool currentWrapping = false;
		if (owner != nullptr)
		{
			if (owner->wrapped)
				currentWrapping = true;
			else
				currentWrapping = wrapped;
		}
		else
		{
			currentWrapping = wrapped;
		}

		switch (Alignment)
		{
		case TopLeft:
			alignPivot = Vector2(0, 0);
			break;
		case Top:
			alignPivot = Vector2(0.5, 0);
			break;
		case TopRight:
			alignPivot = Vector2(1.0, 0);
			break;

		case Left:
			alignPivot = Vector2(0, 0.5);
			break;
		case Center:
			alignPivot = Vector2(0.5, 0.5);
			break;
		case Right:
			alignPivot = Vector2(1.0, 0.5);
			break;

		case BottomLeft:
			alignPivot = Vector2(0, 1.0);
			break;
		case Bottom:
			alignPivot = Vector2(0.5, 1.0);
			break;
		case BottomRight:
			alignPivot = Vector2(1.0, 1.0);
		}

		if (guiType != GUIType::Window && alignPivot != Vector2(-1, -1))
		{
			float font_size_x = 0;

			if(guiType == GUIType::InputField)
				font_size_x = ImGui::CalcTextSize(text.c_str()).x + ImGui::CalcItemWidth();
			else
				font_size_x = ImGui::CalcTextSize(text.c_str()).x;

			float font_size_y = ImGui::CalcTextSize(text.c_str()).y;

			float item_x, item_y;

			if (guiType != Text)
			{
				item_x = (ImGui::GetWindowSize().x - (font_size_x + ImGui::GetStyle().ItemSpacing.x)) * alignPivot.x;
				item_y = (ImGui::GetWindowSize().y - (font_size_y + ImGui::GetStyle().ItemSpacing.y + 2)) * alignPivot.y;
			}
			else
			{
				item_x = (ImGui::GetWindowSize().x - font_size_x) * alignPivot.x;
				item_y = (ImGui::GetWindowSize().y - font_size_y) * alignPivot.y;
			}
			/*float min_x = 20.0f;
			if (item_x <= min_x) {
				item_x = min_x;
			}*/
		
			ImGui::SetCursorPosX(item_x);

			if (itemPos == Absolute)
				ImGui::SetCursorPosY(item_y);

			if (currentWrapping)
				ImGui::PushTextWrapPos(ImGui::GetWindowSize().x - item_x);
		}

		ImGuiStyle& style = ImGui::GetStyle();
		ImColor styleCol = style.Colors[ImGuiCol_Text];
		ImColor windowBgCol = style.Colors[ImGuiCol_WindowBg];
		ImVec2 padding = style.WindowPadding;

		float winBorderSize = style.WindowBorderSize;
		float childWinBorderSize = style.ChildBorderSize;

		style.WindowBorderSize = borderSize;
		style.ChildBorderSize = borderSize;

		float winBorderRadius = style.WindowRounding,
			childWinBorderRadius = style.ChildRounding,
			frameBorderRadius = style.FrameRounding;

		style.WindowRounding = borderRadius;
		style.ChildRounding = borderRadius;
		style.FrameRounding = borderRadius;

		style.WindowPadding = ImVec2(this->padding.x, this->padding.y);

		if (isColored && !IsColorRefUsedForBackground())
		{
			if (colorRef != nullptr)
			{
				style.Colors[ImGuiCol_Text] = ImColor(colorRef->r, colorRef->g, colorRef->b, colorRef->a);
			}
		}
		else
		{
			if (fontColor != Color(-1, -1, -1, -1))
				style.Colors[ImGuiCol_Text] = ImColor(fontColor.r, fontColor.g, fontColor.b, fontColor.a);
		}

		if (UITag == "" && guiType == GUIType::Window)
			textFinal = text + "###" + std::to_string(scriptId);
		else if (UITag == "")
			textFinal = text;
		else
			textFinal = text + "###" + UITag;

		if (font != nullptr)
		{
			ImGui::PushFont(font->font);
		}

		ImGuiWindowFlags window_flags = 0;

		if (lastPosition != Vector2(-1, -1) && lastSize != Vector2(-1, -1))
		{
			if (boxShadow != nullptr)
				RenderBoxShadow();
		}

		ImColor buttonCol, buttonHoverCol, backupChildBgColor;
		float alignW = 0, alignH = 0;
		bool isHovered = false;
		switch (guiType)
		{
		case GUIType::Window:
			if (screenSize.x != -1 || screenSize.y != -1)
				ScaleWithScreenResolution(screenSize);

			if (size.x < 1)
				size.x = 1;
			
			if (size.y < 1)
				size.y = 1;

			if (isColored)
			{
				if (backgroundImage == nullptr)
				{
					if (colorRef != nullptr)
					{
						style.Colors[ImGuiCol_WindowBg] = ImColor(colorRef->r, colorRef->g, colorRef->b, colorRef->a);
					}
				}
			}

			if (backgroundImage != nullptr || backgroundGradients.size() != 0)
				style.Colors[ImGuiCol_WindowBg] = ImColor(0, 0, 0, 0);

			if (owner != nullptr)
			{
				alignW = (ImGui::GetWindowPos().x + ((ImGui::GetWindowSize().x - size.x) * alignPivot.x)) + (localScreenPosition.x * ImGui::GetWindowSize().x);
				alignH = (ImGui::GetWindowPos().y + ((ImGui::GetWindowSize().y - size.y) * alignPivot.y)) + (localScreenPosition.y * ImGui::GetWindowSize().y);
			}
			else
			{
				alignW = Graphics::GetMainWindow().width * alignPivot.x - (size.x / 2) + (localScreenPosition.x * Graphics::GetMainWindow().width);
				alignH = Graphics::GetMainWindow().height * alignPivot.y - (size.y / 2) + (localScreenPosition.y * Graphics::GetMainWindow().height);
			}

			if (alignPivot != Vector2(-1, -1))
			{
				Move(Vector2(
					alignW,
					alignH
				));
			}

			bool window;

			if (!EnableResize) window_flags |= ImGuiWindowFlags_NoResize;
			if (!EnableTitle) window_flags |= ImGuiWindowFlags_NoTitleBar;
			if (!EnableScrolling) window_flags |= ImGuiWindowFlags_NoScrollbar;
			if (!CanBeMoved) window_flags |= ImGuiWindowFlags_NoMove;
			if (containsWindowsInside) { window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus; containsWindowsInside = false; }

			if (!SaveInFile)
				ImGui::GetIO().IniFilename = NULL;

			if (isOpen)
			{
				if (isOpen != _lastOpenState)
				{
					for (int i = 0; i < onOpenEvents.size(); i++)
					{
						onOpenEvents[i]();
					}
					_lastOpenState = isOpen;
				}

				bool isAbsolute = owner != nullptr && itemPos == ItemPosition::Absolute;

				if (_requestForceMove)
				{
					ImVec2 window_next(0, 0);

					if(!isAbsolute && owner != nullptr)
						if(owner->allElements[0] != this)
							window_next = ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + size.y);

					ImGui::SetNextWindowPos(ImVec2(moveToPosition.x + window_next.x, moveToPosition.y + window_next.y), ImGuiCond_Always);
					_requestForceMove = false;
				}

				if (_requestForceScale)
				{
					if (autoResize == Disabled)
						ImGui::SetNextWindowSize(ImVec2(scaleTo.x, scaleTo.y), ImGuiCond_Always);
					else
					{
						if (autoResize == OnlyWidth)
							ImGui::SetNextWindowSize(ImVec2(0, scaleTo.y), ImGuiCond_Always);
						else if(autoResize == OnlyHeight)
							ImGui::SetNextWindowSize(ImVec2(scaleTo.x, 0), ImGuiCond_Always);

					}
					_requestForceScale = false;
				}

				if (owner == nullptr)
				{
					if (!enableOpenAndClose)
						window = ImGui::Begin(textFinal.c_str(), nullptr, window_flags);
					else
						window = ImGui::Begin(textFinal.c_str(), &isOpen, window_flags);
				}
				else
				{
					if(autoResize == Disabled)
						window = ImGui::BeginChild(textFinal.c_str(), ImVec2(size.x, size.y), true);
					else
					{
						if (autoResize == OnlyWidth)
						{
							window = ImGui::BeginChild(textFinal.c_str(), ImVec2(0, size.y), true, ImGuiWindowFlags_AlwaysAutoResize);
						}
						else if (autoResize == OnlyHeight)
						{
							window = ImGui::BeginChild(textFinal.c_str(), ImVec2(size.x, 0), true, ImGuiWindowFlags_AlwaysAutoResize);
						}
					}

					owner->containsWindowsInside = true;
				}

				ImGUIElement::_isMouseOnAnyWindow = false;

				if (window)
				{
					if (ImGui::IsWindowHovered())
						isHovered = true;

					if (ImGui::IsWindowHovered())
						ImGUIElement::_isMouseOnAnyWindow = true;

					ImVec2 window_TopLeft(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
					ImVec2 window_BottomRight(ImGui::GetWindowPos().x + size.x, ImGui::GetWindowPos().y + size.y);

					lastPosition = Vector2(window_TopLeft.x, window_TopLeft.y);
					lastSize = Vector2(size.x, size.y);

					if (owner != nullptr && colorRef != nullptr)
					{
						//if(owner->imgui_drawList != nullptr)
						ImGui::GetWindowDrawList()->AddRectFilled(window_TopLeft, window_BottomRight, ImColor(style.Colors[ImGuiCol_WindowBg]), borderRadius);
					}

					if (backgroundGradients.size() != 0)
					{
						ImGUIElement::RenderGradients(nullptr, backgroundGradients, window_TopLeft, window_BottomRight);
					}

					if (backgroundImage != nullptr)
					{
						if (backgroundImage->IsLoadedToGPU())
						{
							ImVec2 winPad = style.WindowPadding;

							ImVec2 uvMin(float(backgroundImage->finalRect.x) / float(TextureManager::textureGroups[backgroundImage->texGroupId].width),
								float(backgroundImage->finalRect.y) / float(TextureManager::textureGroups[backgroundImage->texGroupId].height));

							ImVec2 uvMax(float(backgroundImage->finalRect.x + backgroundImage->finalRect.width) / float(TextureManager::textureGroups[backgroundImage->texGroupId].width),
								float(backgroundImage->finalRect.y + backgroundImage->finalRect.height) / float(TextureManager::textureGroups[backgroundImage->texGroupId].height));

							if (colorRef == nullptr)
							{
								ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)TextureManager::textureGroups[backgroundImage->texGroupId].texture,
									window_TopLeft,
									window_BottomRight, uvMin, uvMax);
							}
							else
							{
								ImGui::GetWindowDrawList()->AddImage((void*)(intptr_t)TextureManager::textureGroups[backgroundImage->texGroupId].texture,
									window_TopLeft,
									window_BottomRight, uvMin, uvMax, ImColor(colorRef->r, colorRef->g, colorRef->b, colorRef->a));
							}
						}
					}

					for (int i = 0; i < allElements.size(); i++)
					{
						allElements[i]->OnUpdate();
					}

					if (owner == nullptr)
						ImGui::End();
					else
						ImGui::EndChild();
				}
			}
			else
			{
				_lastOpenState = false;
			}

			if (!SaveInFile)
				ImGui::GetIO().IniFilename = "imgui.ini";
			break;

		case GUIType::AppWindow:
			guiType = Window;
			itemPos = Absolute;
			screenSize = Vector2(100, 100);
			ScaleWithScreenResolution(Vector2(0));
			Alignment = Center;
			EnableResize = EnableScrolling = EnableTitle = CanBeMoved = SaveInFile = false;
			break;

		case GUIType::MainMenuBar:
			if (ImGui::BeginMainMenuBar())
			{
				style.Colors[ImGuiCol_Text] = styleCol;
				for (int i = 0; i < allElements.size(); i++)
				{
					allElements[i]->OnUpdate();
				}

				ImGui::EndMainMenuBar();
			}
			break;

		case GUIType::Button:
			buttonCol = style.Colors[ImGuiCol_Button];
			buttonHoverCol = style.Colors[ImGuiCol_ButtonHovered];

			if (colorRef != nullptr)
				style.Colors[ImGuiCol_Button] = ImColor(colorRef->r, colorRef->g, colorRef->b, colorRef->a);

			if(onHoverAnim != nullptr)
				if (colorRef != nullptr)
					style.Colors[ImGuiCol_ButtonHovered] = ImColor(colorRef->r, colorRef->g, colorRef->b, colorRef->a);

			if (ImGui::ButtonEx(textFinal.c_str(), ImVec2(size.x, size.y), ImGuiButtonFlags_FlattenChildren))
			{
				for (int i = 0; i < onClickEvents.size(); i++)
				{
					onClickEvents[i]();
				}
			}

			style.Colors[ImGuiCol_Button] = buttonCol;
			style.Colors[ImGuiCol_ButtonHovered] = buttonHoverCol;
			break;

		case GUIType::Menu:
			if (ImGui::BeginMenu(textFinal.c_str()))
			{
				style.Colors[ImGuiCol_Text] = styleCol;
				for (int i = 0; i < allElements.size(); i++)
				{
					allElements[i]->OnUpdate();
				}
				ImGui::EndMenu();
			}
			break;

		case GUIType::MenuItem:
			if (ImGui::MenuItem(textFinal.c_str()))
			{
				style.Colors[ImGuiCol_Text] = styleCol;
				for (int i = 0; i < allElements.size(); i++)
				{
					allElements[i]->isOpen = true;
				}
			}
			break;

		case GUIType::ListButton:
			if (allElements.size() == 0)
			{
				if (ImGui::Selectable(textFinal.c_str(), &clicked))
				{
					for (int i = 0; i < onClickEvents.size(); i++)
					{
						onClickEvents[i]();
					}
				}
			}
			else
			{
				ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_SpanAvailWidth;

				if (clicked)
				{
					base_flags |= ImGuiTreeNodeFlags_Selected;
				}

				if (ImGui::TreeNodeEx(&UITag, base_flags, text.c_str()))
				{
					if (ImGui::IsItemClicked())
					{
						clicked = true;
					}

					if (clicked)
					{
						for (int i = 0; i < onClickEvents.size(); i++)
						{
							onClickEvents[i]();
						}
					}

					for (int i = 0; i < allElements.size(); i++)
					{
						allElements[i]->OnUpdate();
					}
					ImGui::TreePop();
				}
			}
			break;

		case GUIType::DragVector3:
			ImGui::DragFloat3(textFinal.c_str(), (float*)v4ref, 0.01f);
			v4ref->SendChangeEvent();
			break;

		case GUIType::DragInt:
			ImGui::DragInt(textFinal.c_str(), iRef, 0.1f);
			break;

		case GUIType::InputField:
			backupChildBgColor = style.Colors[ImGuiCol_FrameBg];

			if(colorRef != nullptr)
				style.Colors[ImGuiCol_FrameBg] = ImColor(colorRef->r, colorRef->g, colorRef->b, colorRef->a);

			if (ImGUIElement::InputText(textFinal.c_str(), target, ImGuiInputTextFlags_CallbackResize, InputTextCallback, (void*)target))
			{
				for (int i = 0; i < onChangeEvents.size(); i++)
				{
					onChangeEvents[i]();
				}
			}

			style.Colors[ImGuiCol_FrameBg] = backupChildBgColor;
			break;

		case GUIType::Text:
			if (!currentWrapping)
				ImGui::Text(textFinal.c_str());
			else
			{
				ImGui::TextWrapped(textFinal.c_str());
				if(Alignment == AlignTo::Center)
					ImGui::PopTextWrapPos();
			}
			break;

		case GUIType::ColorEditorButtonRGBA:
			ImGui::ColorEdit4(textFinal.c_str(), (float*)colorRef);
			break;

		case GUIType::ColorEditorButtonRGB:
			ImGui::ColorEdit3(textFinal.c_str(), (float*)colorRef);
			break;

		case GUIType::Columns:

			if (ColumnProperties.DynamicResizing)
			{
				if (ColumnProperties.size == nullptr)
					ColumnProperties.size = new float(256);

				float padding = 16.0f,
					cellSize = *ColumnProperties.size + padding,
					panelWidth = ImGui::GetContentRegionAvail().x;

				int columnCount = (int)(panelWidth / cellSize);

				if (columnCount < 1)
					columnCount = 1;

				val = columnCount;
			}

			ImGui::Columns(val, 0, ColumnProperties.CanResizeManually);
			break;

		case GUIType::EndColumns:
			ImGui::Columns(1);
			break;

		case GUIType::NextColumn:
			ImGui::NextColumn();
			break;

		case GUIType::ImGUIDemoWindow:
			ImGui::ShowDemoWindow();
			break;

		case GUIType::SliderFloat:
			if (ImGui::SliderFloat(textFinal.c_str(), fRef, minVal, maxVal))
			{
				for (int i = 0; i < onChangeEvents.size(); i++)
				{
					onChangeEvents[i]();
				}
			}
			break;

		case GUIType::FileSearch:
			if (ImGui::Button(textFinal.c_str(), ImVec2(size.x, size.y)))
			{
				for (int i = 0; i < onClickEvents.size(); i++)
				{
					onClickEvents[i]();
				}
			}

			ImGui::SameLine();

			if (storedPath != "")
			{
				ImGui::Text(std::experimental::filesystem::path(storedPath).filename().u8string().c_str());
			}
			else
			{
				ImGui::Text(secondText.c_str());
			}
			break;

		case GUIType::SameLine:
			ImGui::SameLine();
			break;

		case GUIType::HelpMarker:
			ImGui::TextDisabled("(?)");
			if (ImGui::IsItemHovered())
			{
				ImGui::BeginTooltip();
				ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
				ImGui::TextUnformatted(text.c_str());
				ImGui::PopTextWrapPos();
				ImGui::EndTooltip();
			}
			break;

		case GUIType::Guizmo:
			if (scriptRef != 0)
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				Matrix transform = scriptRef->transform.GetTransform();

				ImGuiIO& io = ImGui::GetIO();
				ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
				ImGuizmo::Manipulate(
					glm::value_ptr(Graphics::MainCamera()->GetViewProjection().coreMatrix),
					glm::value_ptr(Graphics::MainCamera()->_projectionMatrix.coreMatrix),
					ImGuizmo::TRANSLATE,
					ImGuizmo::LOCAL,
					glm::value_ptr(transform.coreMatrix)
				);
				break;
			}
		}

		RunBroadcastAnimations();

		if (guiType != Window)
			isHovered = ImGui::IsItemHovered();

		if (isHovered)
		{
			if (onHoverOutAnim != nullptr)
				onHoverOutAnim->Stop();

			if (onHoverAnim != nullptr)
				if (!onHoverAnim->IsPlaying() && !isHovering)
					onHoverAnim->Play();

			isHovering = true;
		}
		else
		{
			if (isHovering)
			{
				if (onHoverAnim != nullptr)
					onHoverAnim->Stop();

				if (onHoverOutAnim != nullptr)
					if (!onHoverOutAnim->IsPlaying())
						onHoverOutAnim->Play();

				isHovering = false;
			}
		}

		if (font != nullptr)
		{
			ImGui::PopFont();
		}

		style.Colors[ImGuiCol_Text] = styleCol;
		style.Colors[ImGuiCol_WindowBg] = windowBgCol;

		style.WindowPadding = padding;
		style.WindowBorderSize = winBorderSize;
		style.ChildBorderSize = childWinBorderSize;

		style.WindowRounding = winBorderRadius;
		style.ChildRounding = childWinBorderRadius;
		style.FrameRounding = frameBorderRadius;

		for (int i = 0; i < onUIUpdateEndEvents.size(); i++)
		{
			onUIUpdateEndEvents[i]();
		}
	}
}
