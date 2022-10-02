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

void iGUI::RenderImage(Texture& tex, ImDrawList* drawList, ImVec2 pos, ImVec2 size, Color col)
{
	if(tex.IsLoadedToGPU())
	{
		//std::cout << "Pos: " << pos.x << ", " << pos.y << std::endl;
		//std::cout << "Size: " << size.x << ", " << size.y << std::endl;

		iStyle* current = CurrentStyle();
	
		ImVec2 uvMin(float(tex.finalRect.x) / float(TextureManager::textureGroups[tex.texGroupId].width),
			float(tex.finalRect.y) / float(TextureManager::textureGroups[tex.texGroupId].height));
	
		ImVec2 uvMax(float(tex.finalRect.x + tex.finalRect.width) / float(TextureManager::textureGroups[tex.texGroupId].width),
			float(tex.finalRect.y + tex.finalRect.height) / float(TextureManager::textureGroups[tex.texGroupId].height));
		
		if(col != Color(-1, -1, -1, -1))
		{
			drawList->AddImage((void*)(intptr_t)TextureManager::textureGroups[tex.texGroupId].texture,
				pos,
				ImVec2(pos.x + size.x, pos.y + size.y), 
				uvMin, uvMax, ImColor(col.r * 255, col.g * 255, col.b * 255, col.a * 255));
		}
		else
		{
			drawList->AddImage((void*)(intptr_t)TextureManager::textureGroups[tex.texGroupId].texture,
				pos,
				ImVec2(pos.x + size.x, pos.y + size.y), 
				uvMin, uvMax, ImColor(255, 255, 255, 255));
		}
	}
	else
	{
		std::cout << "Texture is not uploaded to GPU!" << std::endl;
	}
}