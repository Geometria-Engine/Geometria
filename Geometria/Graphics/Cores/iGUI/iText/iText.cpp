#include "iText.h"

void iText::OnUpdate()
{
	iStyle* currentStyle = CurrentStyle();

	currentStyle->UI_PushColors();

	currentStyle->font->font->Scale = float(currentStyle->font->size) / 100.f;
	ImGui::PushFont(currentStyle->font->font);

	ImGui::Text(text.c_str());

	ImGui::PopFont();

	currentStyle->UI_PopColors();
}