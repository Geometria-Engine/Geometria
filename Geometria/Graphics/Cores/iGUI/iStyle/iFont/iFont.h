#ifndef I_FONT_H
#define I_FONT_H

#include <string>
struct ImFont;
struct ImFontConfig;

struct iFont
{
	std::string file, _lastFile;
	int size = 0, _lastSize = 0;
	static ImFont* defaultFont;
	ImFont* font = nullptr;

	float _alignment = 0.f;

	enum WordWrap
	{
		None,
		BreakSpace,
		BreakRawSpace
	};

	WordWrap _ww = WordWrap::None;

	WordWrap& Wrap()
	{
		return _ww;
	}

	float& Align()
	{
		return _alignment;
	}

	float AlignToLeft() { return 0.f; }
	float AlignToCenter() { return 0.5f; }
	float AlignToRight() { return 1.f; }

	iFont()
	{

	}

	void ImGuiBegin();

	int& Size()
	{
		return size;
	}

	void ImGuiEnd()
	{
		//ImGui::PopFont();
	}

	// ImGUI Modifications

	//static ImFont* ImGui_AddFontFromFileTTF(const char* filename, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges);
	//static ImFont* ImGui_AddFontFromMemoryTTF(void* ttf_data, int ttf_size, float size_pixels, const ImFontConfig* font_cfg_template, const ImWchar* glyph_ranges);
	//static ImFont* ImGui_AddFont(const ImFontConfig* font_cfg);
};

#endif