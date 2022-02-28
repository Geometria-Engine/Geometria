#ifndef GUIML_H
#define GUIML_H
#include "Graphics/Externals/ImGUIElement.h"
#include "../XML/pugixml.hpp"
#include "CSS/CSSParser.h"

struct GUIML
{
	struct GUIMLCSSStyle
	{
		std::string
			className,
			width,
			height,
			maxWidth,
			maxHeight,
			minWidth,
			minHeight,
			alignment,
			fontFamily,
			fontSize,
			borderWidth,
			backgroundImage,
			image,
			paddingX,
			paddingY,
			wrapping,
			positionType,
			windowTitle,
			windowResize,
			borderRadius,
			boxShadowX,
			boxShadowY,
			boxShadowBlur,
			autoResize;

		std::vector<std::string> animations, onHoverAnimations, onHoverUpAnimations;
		std::vector<std::pair<std::vector<std::string>, std::string>> bcastAnimations;

		Color
			color = Color(-1, -1, -1, -1),
			backgroundColor = Color(-1, -1, -1, -1),
			boxShadowColor = Color(-1, -1, -1, -1);

		std::vector<std::pair<Color, float>> backgroundGradient;
	};

	struct GUIMLCSSFont
	{
		std::string url, name;
	};

	static Color RGBToCol(std::string text);

	static ImGUIElement* NewGUIML(std::string url);
	static ImGUIElement* ReadGUIMLNode(pugi::xml_node node, ImGUIElement* owner);

	static void ApplyBroadcast(ImGUIElement& gui, pugi::xml_node node);

	static void ApplyCSS(ImGUIElement& gui, std::string c);
	static void CSS_ApplyResolution(ImGUIElement& gui, std::string v, std::string value);
	static void CSS_ApplyMaxOrMinResolution(ImGUIElement& gui, std::string v, std::string value);
	static void CSS_ApplyAlignment(ImGUIElement& gui, std::string value);
	static void CSS_ApplyFont(ImGUIElement& gui, GUIMLCSSFont f);
	static void CSS_ApplyColor(ImGUIElement& gui, Color col);
	static void CSS_ApplyBackgroundColor(ImGUIElement& gui, Color col);

	static void CSS_ApplyBorderWidth(ImGUIElement& gui, std::string width);
	static void CSS_ApplyBorderRadius(ImGUIElement& gui, std::string radius);
	static void CSS_ApplyBackgroundImage(ImGUIElement& gui, std::string url);

	static void CSS_ApplyPositionType(ImGUIElement& gui, std::string type);

	static void CSS_ApplyAutoResize(ImGUIElement& gui, std::string ar);

	static void CSS_ApplyAnimation(ImGUIElement& gui, std::vector<std::string> commands, bool playOnStart);
	static void CSS_ApplyAnimation(ImGUIElement& gui, std::vector<std::string> commands, bool playOnStart, std::string type);
	static void CSS_Anim_ApplyProperties(Animation::Timeline& t, ImGUIElement& gui, std::pair<std::string, std::string> css, int frame, int duration, Animation::Easing e);
	static Animation::Easing CSS_Anim_GetEasing(std::string easing);

	static void CSS_Anim_Size(Animation::Timeline& t, ImGUIElement& gui, std::string widthOrHeight, std::string v, int frame, int duration, Animation::Easing e);
	static void CSS_Anim_Color(Animation::Timeline& t, ImGUIElement& gui, Color col, int frame, int duration, Animation::Easing e);
	static void CSS_Anim_BackgroundColor(Animation::Timeline& t, ImGUIElement& gui, Color col, int frame, int duration, Animation::Easing e);
	static void CSS_Anim_Align(Animation::Timeline& t, ImGUIElement& gui, std::string v, int frame, int duration, Animation::Easing e);

	static std::vector<std::pair<Color, float>> TextToGradient(std::string content);

	static std::vector<GUIMLCSSStyle> _currentCSSStyles;
	static std::vector<GUIMLCSSFont> _currentCSSFonts;

	static float PixelsToFloat(std::string val);
	static bool TextToBool(std::string val);

	struct NodeInfo
	{
		std::string nodeName, nodeClass;

		NodeInfo(pugi::xml_node p)
		{
			nodeName = p.attribute("name").as_string();
			nodeClass = p.attribute("class").as_string();
		}
	};
};
#endif