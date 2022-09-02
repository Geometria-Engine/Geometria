#include "iText.h"

void iText::OnUpdate()
{
	if(style != nullptr)
	{
		style->font->font->Scale = float(style->font->size) / 100.f;
		ImGui::PushFont(style->font->font);
	}
	else
	{
		iGUI::GlobalStyle()->font->font->Scale = float(iGUI::GlobalStyle()->font->size) / 100.f;
		ImGui::PushFont(iGUI::GlobalStyle()->font->font);
	}

	ImGui::Text(text.c_str());

	ImGui::PopFont();
}