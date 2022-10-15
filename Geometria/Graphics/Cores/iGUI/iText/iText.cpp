#include "iText.h"
#include "String/StringAPI.h"

void iText::OnUpdate()
{
	iStyle* currentStyle = CurrentStyle();

	currentStyle->UI_PushColors();

	currentStyle->font->font->Scale = float(currentStyle->font->size) / 100.f;
	ImGui::PushFont(currentStyle->font->font);

	std::vector<std::string> lines;

	if(currentStyle->font->Wrap() == iFont::WordWrap::None)
	{
		lines.push_back(text);
	}
	else if(currentStyle->font->Wrap() == iFont::WordWrap::BreakSpace || 
		currentStyle->font->Wrap() == iFont::WordWrap::BreakRawSpace)
	{
		std::vector<std::string> words = StringAPI::SplitIntoVector(text, " ");

		float totalLineSize = 0;

		for(int i = 0; i < words.size(); i++)
		{
			if(lines.size() == 0)
				lines.push_back(std::string());

			lines.back() += words[i];
			lines.back() += " ";

			int wordSize = ImGui::CalcTextSize(words[i].c_str()).x;

			if(currentStyle->font->Wrap() == iFont::WordWrap::BreakSpace && i + 1 < words.size())
				wordSize += ImGui::CalcTextSize(words[i + 1].c_str()).x;

			float result = totalLineSize + wordSize;
			float condition = owner->_trueScale.x * ((owner->_trueScale.x / ((owner->_trueScale.x + wordSize) / 2)) - currentStyle->font->font->Scale);

			if(result >= condition)
			{
				lines.push_back(std::string());
				totalLineSize = 0;
			}
			else
				totalLineSize += wordSize;
		}
	}

	for(auto i : lines)
	{
		float cursorLinePos = owner->_trueScale.x - ImGui::CalcTextSize(i.c_str()).x;
		ImGui::SetCursorPosX(cursorLinePos - (cursorLinePos * (1 - currentStyle->font->Align())));
		ImGui::Text(i.c_str());
	}

	ImGui::PopFont();

	currentStyle->UI_PopColors();
}