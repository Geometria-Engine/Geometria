#include "iValue.h"

void iValue::OnUpdate()
{
	iStyle* currentStyle = CurrentStyle();
	
	currentStyle->font->font->Scale = float(currentStyle->font->size) / 100.f;
	ImGui::PushFont(currentStyle->font->font);

	if(_intP != nullptr)
		ImGui::DragInt(title.c_str(), _intP, speed);
	else if(_floatP != nullptr)
		ImGui::DragFloat(title.c_str(), _floatP, speed);

	else if(_vec3P != nullptr)
		ImGui::DragFloat3(title.c_str(), (float*)_vec3P, speed);

	ImGui::PopFont();
}