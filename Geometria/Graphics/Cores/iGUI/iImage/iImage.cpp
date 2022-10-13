#include "iImage.h"

void iImage::Image_SetParams()
{
	iStyle* currentStyle = CurrentStyle();

	int alignW, alignH, scaleW, scaleH;
	int ownerWidth, ownerHeight, ownerX, ownerY;

	if(owner == nullptr)
	{
		ownerWidth = Graphics::GetMainWindow().width;
		ownerHeight = Graphics::GetMainWindow().height;
	}
	else
	{
		ownerWidth = owner->GetTransform().scale.x;
		ownerHeight = owner->GetTransform().scale.y;

		ownerX = owner->GetTransform().position.x;
		ownerY = owner->GetTransform().position.y;
	}

	if(currentStyle->_screenScale != Vector2(-1, -1))
	{
		scaleW = ((float)currentStyle->_screenScale.x / 100.f) * ownerWidth + GetTransform().scale.x;
		scaleH = ((float)currentStyle->_screenScale.y / 100.f) * ownerHeight + GetTransform().scale.y;
	}
	else
	{
		scaleW = GetTransform().scale.x;
		scaleH = GetTransform().scale.y;
	}

	if(currentStyle->_screenPosition != Vector2(-1, -1))
	{
		alignW = ownerWidth * currentStyle->_screenPosition.x - (scaleW / (1 / currentStyle->_screenPosition.x)) + ((GetTransform().position.x + ownerX) / ownerWidth) + GetTransform().position.x + ownerX;
		alignH = ownerHeight * currentStyle->_screenPosition.y - (scaleH / (1 / currentStyle->_screenPosition.y)) + ((GetTransform().position.y + ownerY) / ownerHeight) - GetTransform().position.y + ownerY;
	}
	else
	{
		alignW = GetTransform().position.x;
		alignH = GetTransform().position.y;
	}

	_truePos = Vector2(alignW, alignH);
	_trueSca = Vector2(scaleW, scaleH);
}

void iImage::OnUpdate()
{
	iStyle* currentStyle = CurrentStyle();

	Image_SetParams();
	RenderImage(*_texture, ImGui::GetWindowDrawList(), ImVec2(_truePos.x, _truePos.y), ImVec2(_trueSca.x, _trueSca.y), currentStyle->MainColor());
}