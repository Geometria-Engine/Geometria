#include "GUIML.h"
#include "Files\Files.h"
#include "String\StringAPI.h"
#include "Graphics/Cores/MainAPI/Graphics.h"
#include <Broadcast\Broadcast.h>
#include <Web\Web.h>

std::vector<GUIML::GUIMLCSSStyle> GUIML::_currentCSSStyles;
std::vector<GUIML::GUIMLCSSFont> GUIML::_currentCSSFonts;

Color GUIML::RGBToCol(std::string text)
{
	if (text.find("rgb(") != std::string::npos || text.find("rgba(") != std::string::npos)
	{
		std::string valuesInText = text;
		valuesInText = StringAPI::GetSubstringBetween(valuesInText, "rg", ")");
		valuesInText = StringAPI::RemoveAll(valuesInText, "b(");
		valuesInText = StringAPI::RemoveAll(valuesInText, "ba(");
		valuesInText = StringAPI::RemoveAll(valuesInText, " ");
		valuesInText += ",";
		std::vector<std::string> values = StringAPI::SplitIntoVector(valuesInText, ",");

		Color finalColor;
		int count = 0;
		for (int i = 0; i < values.size(); i++)
		{
			if (values[i] != "")
			{
				if (i == 0)
				{
					finalColor.r = std::stof(values[i]);
					if (finalColor.r > 1)
						finalColor.r /= 255.0f;
				}
				else if (i == 1)
				{
					finalColor.g = std::stof(values[i]);
					if (finalColor.g > 1)
						finalColor.g /= 255.0f;
				}
				else if (i == 2)
				{
					finalColor.b = std::stof(values[i]);
					if (finalColor.b > 1)
						finalColor.b /= 255.0f;
				}
				else if (i == 3)
				{
					finalColor.a = std::stof(values[i]);
					if (finalColor.a > 1)
						finalColor.a /= 255.0f;
				}
				count = i;
			}
		}

		if (count != 3)
			finalColor.a = 1;

		return finalColor;
	}

	return Color();
}

ImGUIElement* GUIML::NewGUIML(std::string url)
{
	std::string rawGuiml, cssUrl;
	bool isWeb = false;

	if (StringAPI::StartsWith(url, "http")) { rawGuiml = Web::Get(url.c_str(), false); isWeb = true; }
	else
		rawGuiml = Files::Read(url.c_str());

	pugi::xml_document doc;
	pugi::xml_parse_result doc_result = doc.load_string(rawGuiml.c_str());

	_currentCSSStyles.clear();
	std::vector<GUIML::GUIMLCSSStyle>().swap(_currentCSSStyles);

	if (rawGuiml.find("<main-style src=") != std::string::npos)
	{
		cssUrl = StringAPI::GetSubstringBetween(rawGuiml, "<main-style src=", "/>");

		cssUrl = StringAPI::RemoveAll(cssUrl, "\" ");
		cssUrl = StringAPI::RemoveAll(cssUrl, "\"");
	}

	if (cssUrl != "")
	{
		std::string cssResult;
		std::vector<std::string> cssLines;

		if (isWeb)
		{
			if (!StringAPI::StartsWith(cssUrl, "http"))
			{
				std::string newCssUrl = "http";
				newCssUrl += StringAPI::GetSubstringBetween(url, "http", "/rfp");
				newCssUrl += "/rfp/";
				newCssUrl += cssUrl;

				cssUrl = newCssUrl;
			}

			std::string webCss = Web::Get(cssUrl.c_str(), false);
			cssLines = StringAPI::GetLinesFromString(webCss);
		}
		else
			cssLines = Files::ReadAndGetLines(cssUrl.c_str());

		bool sameLine = false;

		for (auto i : cssLines)
		{
			sameLine = i.find("|") != std::string::npos;

			std::string checkIfLineIsEmpty = StringAPI::RemoveAll(i, "\n");
			checkIfLineIsEmpty = StringAPI::RemoveAll(checkIfLineIsEmpty, "\t");

			if (i.find(":") != std::string::npos && i.find(";") != std::string::npos)
			{
				std::string getValue = StringAPI::GetSubstringBetween(i, ": ", ";");
				std::string replacedValue = StringAPI::ReplaceAll(getValue, " ", "?");
				std::string result = StringAPI::ReplaceAll(i, getValue, replacedValue);
				i = result;
			}

			if (checkIfLineIsEmpty != "")
			{
				if (!sameLine)
				{
					if (i.find(": ") == std::string::npos || i.find(";") == std::string::npos)
					{
						i = StringAPI::RemoveAll(i, "\t");
						i = StringAPI::ReplaceAll(i, " ", "?");
					}
					i += "\n";
				}
				else
				{
					i = StringAPI::ReplaceAll(i, " ", "?");
					i = StringAPI::ReplaceAll(i, ":?", ": ");
					i = StringAPI::RemoveAll(i, "\n");
					i = StringAPI::RemoveAll(i, "\t");
				}
				cssResult += i;
			}
		}

		std::cout << cssResult << std::endl;

		CSSParser prs = CSSParser(cssResult);
		while (!prs.AtEnd())
		{
			CSSToken tok = prs.GetCSSToken();

			if (tok.Selector.Type == SelectorType::Class)
			{
				GUIML::GUIMLCSSStyle addClass;

				addClass.className = tok.Selector.Subject;

				for (std::pair<string, CSSStyle> pair : tok.Styles)
				{
					pair.second.Text = StringAPI::ReplaceAll(pair.second.Text, "?", " ");

					// Resolutions
					if (pair.first == "width")
						addClass.width = pair.second.Text;
					else if (pair.first == "height")
						addClass.height = pair.second.Text;

					// Max Resolutions
					else if (pair.first == "max-width")
						addClass.maxWidth = pair.second.Text;
					else if (pair.first == "max-height")
						addClass.maxHeight = pair.second.Text;

					// Min Resolutions
					else if (pair.first == "min-width")
						addClass.minWidth = pair.second.Text;
					else if (pair.first == "min-height")
						addClass.minHeight = pair.second.Text;

					// Align
					else if (pair.first == "align")
						addClass.alignment = pair.second.Text;

					// Font
					else if (pair.first == "font-family")
					{
						if (pair.second.Text.find("\"") != std::string::npos)
							addClass.fontFamily = StringAPI::RemoveAll(pair.second.Text, "\"");
					}
					else if (pair.first == "font-size")
						addClass.fontSize = pair.second.Text;

					// Color
					else if (pair.first == "color" || pair.first == "colour")
						addClass.color = GUIML::RGBToCol(pair.second.Text);
					else if (pair.first == "background-color" || pair.first == "background-colour")
					{
						if (pair.second.Text.find("linear-gradient(") != std::string::npos)
						{
							std::string finalC = pair.second.Text;
							finalC += "!";
							addClass.backgroundGradient = GUIML::TextToGradient(finalC);
						}
						else
							addClass.backgroundColor = GUIML::RGBToCol(pair.second.Text);
					}

					// Border Width
					else if (pair.first == "border-width")
						addClass.borderWidth = pair.second.Text;

					// Border Radius
					else if (pair.first == "border-radius")
						addClass.borderRadius = pair.second.Text;

					// Image
					else if (pair.first == "background-image")
						addClass.backgroundImage = StringAPI::RemoveAll(pair.second.Text, "\"");

					// Padding
					else if (pair.first == "padding-x")
						addClass.paddingX = pair.second.Text;
					else if (pair.first == "padding-y")
						addClass.paddingY = pair.second.Text;

					// Text Wrapping
					else if (pair.first == "wrapping")
						addClass.wrapping = pair.second.Text;

					// Position
					else if (pair.first == "position")
						addClass.positionType = pair.second.Text;

					// Window Title
					else if (pair.first == "enable-title")
						addClass.windowTitle = pair.second.Text;

					// Window Resize
					else if (pair.first == "enable-resize")
						addClass.windowResize = pair.second.Text;

					// [---------------EFFECTS---------------]
					// Box Shadow
					else if (pair.first == "box-shadow-color" || pair.first == "box-shadow-colour")
						addClass.boxShadowColor = RGBToCol(pair.second.Text);

					else if (pair.first == "box-shadow-x")
						addClass.boxShadowX = pair.second.Text;
					else if (pair.first == "box-shadow-y")
						addClass.boxShadowY = pair.second.Text;

					else if (pair.first == "box-shadow-blur")
						addClass.boxShadowBlur = pair.second.Text;
					// [-------------------------------------]

					// Animation
					else if (pair.first == "animation")
						addClass.animations = StringAPI::SplitIntoVector(pair.second.Text, "|");
					else if (pair.first == "onhover")
						addClass.onHoverAnimations = StringAPI::SplitIntoVector(pair.second.Text, "|");
					else if (pair.first == "onhoverup")
						addClass.onHoverUpAnimations = StringAPI::SplitIntoVector(pair.second.Text, "|");
					else if (pair.first.find("broadcast[") != std::string::npos)
					{
						addClass.bcastAnimations.push_back(
							std::make_pair(
								StringAPI::SplitIntoVector(pair.second.Text, "|"),
								StringAPI::GetSubstringBetween(pair.first, "broadcast[", "]")));
					}

					// Auto Resize
					else if (pair.first == "auto-resize")
						addClass.autoResize = pair.second.Text;
				}

				_currentCSSStyles.push_back(addClass);
			}
			else if (tok.Selector.Type == SelectorType::Tag)
			{
				if (tok.Selector.Subject == "@font-face")
				{
					GUIML::GUIMLCSSFont addFont;

					for (std::pair<string, CSSStyle> pair : tok.Styles)
					{
						pair.second.Text = StringAPI::ReplaceAll(pair.second.Text, "?", " ");

						if (pair.first == "font-family")
						{
							if (pair.second.Text.find("\"") != std::string::npos)
								addFont.name = StringAPI::RemoveAll(pair.second.Text, "\"");
						}
						else if (pair.first == "src")
						{
							if (pair.second.Text.find("\"") != std::string::npos)
								addFont.url = StringAPI::RemoveAll(pair.second.Text, "\"");
						}
					}

					_currentCSSFonts.push_back(addFont);
				}
			}
		}
	}

	if (doc_result)
	{
		return ReadGUIMLNode(doc, nullptr);
	}

	return nullptr;
}

ImGUIElement* GUIML::ReadGUIMLNode(pugi::xml_node node, ImGUIElement* owner)
{
	ImGUIElement* Node = nullptr;

	auto guiml = node.child("guiml");

	NodeInfo GUIMLInfo(guiml);

	if (guiml)
	{
		std::string windowType = guiml.attribute("type").as_string();

		if(windowType == "Window")
			Node = new ImGUIElement(ImGUIElement::GUIType::Window, GUIMLInfo.nodeName);
		else if(windowType == "App")
			Node = new ImGUIElement(ImGUIElement::GUIType::AppWindow, GUIMLInfo.nodeName);
		else if(windowType == "")
			Node = new ImGUIElement(ImGUIElement::GUIType::AppWindow, GUIMLInfo.nodeName);

		ApplyCSS(*Node, GUIMLInfo.nodeClass);

		for (pugi::xml_node subnode : guiml)
			ReadGUIMLNode(subnode, Node);
	}

	if (owner != nullptr)
	{
		std::ostringstream content;
		node.print(content);

		pugi::xml_document ok;
		pugi::xml_parse_result result = ok.load_string(content.str().c_str());

		auto p = ok.child("p");
		auto cont = ok.child("cont");
		auto input = ok.child("input");
		auto div = ok.child("div");
		auto button = ok.child("button");
		if (p)
		{
			NodeInfo nodeInfo(p);

			Node = new ImGUIElement(ImGUIElement::GUIType::Text, *owner, p.text().as_string());
			ApplyCSS(*Node, nodeInfo.nodeClass);

			for (pugi::xml_node subnode : p)
				ReadGUIMLNode(subnode, Node);
		}
		else if (cont)
		{
			Node = new ImGUIElement(ImGUIElement::GUIType::SameLine, *owner);
			for (pugi::xml_node subnode : cont)
				ReadGUIMLNode(subnode, Node);
		}
		else if (input)
		{
			NodeInfo nodeInfo(input);

			Node = new ImGUIElement(ImGUIElement::GUIType::InputField, *owner, nodeInfo.nodeName, std::string());
			Node->target = &Node->strData;
			ApplyCSS(*Node, nodeInfo.nodeClass);

			std::string b = input.attribute("broadcast").as_string();
			if (b == "true")
				Broadcast::AddImGUIElementToBroadcast(Node, nodeInfo.nodeName);

			for (pugi::xml_node subnode : input)
				ReadGUIMLNode(subnode, Node);
		}
		else if (div)
		{
			NodeInfo nodeInfo(div);

			Node = new ImGUIElement(ImGUIElement::GUIType::Window, *owner, nodeInfo.nodeName);
			Node->EnableTitle = Node->EnableResize = false;
			ApplyCSS(*Node, nodeInfo.nodeClass);

			for (pugi::xml_node subnode : div)
				ReadGUIMLNode(subnode, Node);
		}
		else if (button)
		{
			NodeInfo nodeInfo(button);

			Node = new ImGUIElement(ImGUIElement::GUIType::Button, *owner, button.text().as_string());
			Node->target = &Node->strData;
			ApplyCSS(*Node, nodeInfo.nodeClass);
			ApplyBroadcast(*Node, button);

			for (pugi::xml_node subnode : button)
				ReadGUIMLNode(subnode, Node);
		}
	}

	return Node;
}

void GUIML::ApplyBroadcast(ImGUIElement& gui, pugi::xml_node node)
{
	std::string onclick = node.attribute("onclick").as_string();
	
	if (onclick.find("broadcast(") != std::string::npos)
	{
		gui.BroadcastOnClick(StringAPI::GetSubstringBetween(onclick, "broadcast('", "')"));
	}
}

void GUIML::ApplyCSS(ImGUIElement& gui, std::string c)
{
	std::vector<GUIMLCSSStyle> guimlCss;
	GUIMLCSSFont getFont;

	for (auto style : _currentCSSStyles)
	{
		if (style.className == c)
		{
			guimlCss.push_back(style);
		}
	}

	for (auto style : guimlCss)
	{
		for (auto font : _currentCSSFonts)
		{
			if (font.name == style.fontFamily)
			{
				getFont = font;
			}
		}
	}

	for (auto css : guimlCss)
	{
		if (css.width != "")
			CSS_ApplyResolution(gui, "width", css.width);
		if (css.height != "")
			CSS_ApplyResolution(gui, "height", css.height);
		if (css.alignment != "")
			CSS_ApplyAlignment(gui, css.alignment);

		if (css.fontSize != "")
			gui.fontSize = PixelsToFloat(css.fontSize);
		if (css.fontFamily != "")
			CSS_ApplyFont(gui, getFont);

		if (css.color != Color(-1, -1, -1, -1))
			CSS_ApplyColor(gui, css.color);
		if (css.backgroundColor != Color(-1, -1, -1, -1))
			CSS_ApplyBackgroundColor(gui, css.backgroundColor);
		if (css.backgroundGradient.size() != 0)
			gui.AddBackgroundGradient(css.backgroundGradient);

		if (css.borderWidth != "")
			CSS_ApplyBorderWidth(gui, css.borderWidth);
		if (css.borderRadius != "")
			CSS_ApplyBorderRadius(gui, css.borderRadius);

		if (css.maxWidth != "")
			CSS_ApplyMaxOrMinResolution(gui, "max-width", css.maxWidth);
		if (css.maxHeight != "")
			CSS_ApplyMaxOrMinResolution(gui, "max-height", css.maxHeight);
		if (css.minWidth != "")
			CSS_ApplyMaxOrMinResolution(gui, "min-width", css.minWidth);
		if (css.minHeight != "")
			CSS_ApplyMaxOrMinResolution(gui, "min-height", css.minHeight);

		if (css.backgroundImage != "")
			CSS_ApplyBackgroundImage(gui, css.backgroundImage);

		if (css.paddingX != "")
			gui.padding.x = PixelsToFloat(css.paddingX);
		if (css.paddingY != "")
			gui.padding.y = PixelsToFloat(css.paddingY);

		if (css.wrapping != "")
			gui.wrapped = TextToBool(css.wrapping);

		if (css.positionType != "")
			CSS_ApplyPositionType(gui, css.positionType);

		if (css.windowTitle != "")
			gui.EnableTitle = TextToBool(css.windowTitle);

		if (css.windowResize != "")
			gui.EnableResize = TextToBool(css.windowResize);

		float bSX = 0, bSY = 0, bSBlur = 0;
		if (css.boxShadowX != "")
			bSX = PixelsToFloat(css.boxShadowX);
		if (css.boxShadowY != "")
			bSY = PixelsToFloat(css.boxShadowY);
		if (css.boxShadowBlur != "")
			bSBlur = PixelsToFloat(css.boxShadowBlur);

		if (css.boxShadowColor != Color(-1, -1, -1, -1))
			gui.AddBoxShadow(css.boxShadowColor, Vector2(bSX, bSY), bSBlur);

		if (css.animations.size() != 0)
			CSS_ApplyAnimation(gui, css.animations, true);
		if (css.onHoverAnimations.size() != 0)
			CSS_ApplyAnimation(gui, css.onHoverAnimations, false, "onhover");
		if (css.onHoverUpAnimations.size() != 0)
			CSS_ApplyAnimation(gui, css.onHoverUpAnimations, false, "onhoverup");

		for (auto i : css.bcastAnimations)
		{
			std::cout << "Applying Broadcast " << i.second << '\n';
			CSS_ApplyAnimation(gui, i.first, false, i.second);
		}

		if (css.autoResize != "")
			CSS_ApplyAutoResize(gui, css.autoResize);
	}
}

void GUIML::CSS_ApplyResolution(ImGUIElement& gui, std::string var, std::string value)
{
	int finalW = -1, finalH = -1;
	bool screen = false;
	if (var == "width")
	{
		if (value.find("px") != std::string::npos)
		{
			float w = PixelsToFloat(value);
			gui.size.x = w;
			screen = false;
		}
		else if (value.find("vw") != std::string::npos)
		{
			screen = true;
			std::string res = StringAPI::ReplaceAll(value, "vw", "");
			finalW = std::stoi(res);
			gui.screenSize.x = finalW;
		}
	}

	if (var == "height")
	{
		if (value.find("px") != std::string::npos)
		{
			float h = PixelsToFloat(value);
			gui.size.y = h;
			screen = false;
		}
		else if (value.find("vh") != std::string::npos)
		{
			screen = true;
			std::string res = StringAPI::ReplaceAll(value, "vh", "");
			finalH = std::stoi(res);
			gui.screenSize.y = finalH;
		}
	}

	if (screen)
	{
		std::cout << "Screen set to true!" << '\n';
		gui.ScaleWithScreenResolution(gui.screenSize);
	}
	else
		gui.Scale(gui.size);
}

void GUIML::CSS_ApplyMaxOrMinResolution(ImGUIElement& gui, std::string var, std::string value)
{
	float finalW = -1, finalH = -1;
	if (var == "max-width")
	{
		finalW = PixelsToFloat(value);
		gui.maxScale.x = finalW;
	}

	if (var == "max-height")
	{
		finalH = PixelsToFloat(value);
		gui.maxScale.y = finalH;
	}

	if (var == "min-width")
	{
		finalW = PixelsToFloat(value);
		gui.minScale.x = finalW;
	}

	if (var == "min-height")
	{
		finalH = PixelsToFloat(value);
		gui.minScale.y = finalH;
	}
}

void GUIML::CSS_ApplyAlignment(ImGUIElement& gui, std::string value)
{
	if (value.find("%") == std::string::npos)
	{
		if (value == "top-left")
			gui.Alignment = ImGUIElement::AlignTo::TopLeft;
		else if (value == "top-right")
			gui.Alignment = ImGUIElement::AlignTo::TopRight;
		else if (value == "bottom-left")
			gui.Alignment = ImGUIElement::AlignTo::BottomLeft;
		else if (value == "bottom-right")
			gui.Alignment = ImGUIElement::AlignTo::BottomRight;

		else if (value == "left")
			gui.Alignment = ImGUIElement::AlignTo::Left;
		else if (value == "center")
			gui.Alignment = ImGUIElement::AlignTo::Center;
		else if (value == "right")
			gui.Alignment = ImGUIElement::AlignTo::Right;
		else if (value == "top")
			gui.Alignment = ImGUIElement::AlignTo::Top;
		else if (value == "bottom")
			gui.Alignment = ImGUIElement::AlignTo::Bottom;
	}
	else
	{
		std::vector<std::string> values = StringAPI::SplitIntoVector(value, " ");
		if (values.size() == 2)
		{
			float x = std::stof(StringAPI::RemoveAll(values[0], "%"));
			x /= 100.0f;

			float y = std::stof(StringAPI::RemoveAll(values[1], "%"));
			y /= 100.0f;

			gui.SetAlignPivot(Vector2(x, y));
		}
	}
}

void GUIML::CSS_ApplyFont(ImGUIElement& gui, GUIMLCSSFont f)
{
	ImGUIEFont* font = ImGUIElement::GetFont(f.name.c_str(), gui.fontSize);

	if (font != nullptr)
		gui.font = font;
	else if (font == nullptr)
	{
		font = ImGUIElement::AddFont(f.name.c_str(), f.url.c_str(), gui.fontSize);
		gui.font = font;
	}
}

void GUIML::CSS_ApplyColor(ImGUIElement& gui, Color col)
{
	gui.SetFontColor(col);
}

void GUIML::CSS_ApplyBackgroundColor(ImGUIElement& gui, Color col)
{
	gui.SetColor(col);
}

void GUIML::CSS_ApplyBorderWidth(ImGUIElement& gui, std::string width)
{
	gui.borderSize = PixelsToFloat(width);
}

void GUIML::CSS_ApplyBorderRadius(ImGUIElement& gui, std::string radius)
{
	gui.borderRadius = PixelsToFloat(radius);
}

void GUIML::CSS_ApplyBackgroundImage(ImGUIElement& gui, std::string url)
{
	gui.backgroundImage = new Texture(url.c_str(), Texture::Type::Default);
}

void GUIML::CSS_ApplyPositionType(ImGUIElement& gui, std::string type)
{
	if (type == "relative")
		gui.itemPos = ImGUIElement::ItemPosition::Relative;
	else if (type == "absolute")
		gui.itemPos = ImGUIElement::ItemPosition::Absolute;
}

void GUIML::CSS_ApplyAutoResize(ImGUIElement& gui, std::string ar)
{
	if (ar == "disabled")
		gui.autoResize = ImGUIElement::AutoResize::Disabled;
	else if (ar == "only-width")
		gui.autoResize = ImGUIElement::AutoResize::OnlyWidth;
	else if (ar == "only-height")
		gui.autoResize = ImGUIElement::AutoResize::OnlyHeight;
}

void GUIML::CSS_ApplyAnimation(ImGUIElement& gui, std::vector<std::string> commands, bool playOnStart)
{
	CSS_ApplyAnimation(gui, commands, playOnStart, "");
}

void GUIML::CSS_ApplyAnimation(ImGUIElement& gui, std::vector<std::string> commands, bool playOnStart, std::string type)
{
	Animation::Timeline* newT = nullptr;
	int frame = 0, duration = 0;
	std::string easing;

	for (auto i : commands)
	{
		if (i.find("begin") != std::string::npos)
		{
			std::string frString = StringAPI::GetSubstringBetween(i, "begin(", ")");
			int framerate = std::stoi(frString);

			newT = Animation::New(framerate);

			std::cout << "Animation Summoned!" << '\n';
		}
		else if (i.find("new-frame") != std::string::npos && newT != nullptr)
		{
			std::string get = StringAPI::GetSubstringBetween(i, "new-frame(", ")");

			// Get Frame, Duration and Easing
			std::vector<std::string> values = StringAPI::SplitIntoVector(get, ",");
			if (values.size() >= 2)
			{
				if (values[0].find("s") != std::string::npos)
				{
					values[0] = StringAPI::RemoveAll(values[0], "s");
					frame = newT->InSeconds(std::stof(values[0]));
				}
				else
				{
					frame = std::stoi(values[0]);
				}

				if (values[1].find("s") != std::string::npos)
				{
					values[1] = StringAPI::RemoveAll(values[1], "s");
					duration = newT->InSeconds(std::stof(values[1]));
				}
				else
				{
					duration = std::stoi(values[1]);
				}

				if (values.size() > 2)
				{
					easing = values[2];
				}
				/*else
				{
					std::cout << "No Easing Value! Default to Linear!" << "\n";
				}*/
			}

			//std::cout << "New Frame Summoned!" << '\n';
		}
		else if (i.find("add-value") != std::string::npos)
		{
			if (newT == nullptr)
			{
				newT = Animation::New(70);
				frame = 0;
				duration = 1;
			}

			std::string get = StringAPI::GetSubstringBetween(i, "add-value(", ")");
			std::vector<std::string> v = StringAPI::SplitIntoVector(get, " = ");
			std::pair<std::string, std::string> css;
			if (v.size() == 2)
			{
				css.first = v[0];
				css.second = v[1];

				std::cout << css.first << " || " << css.second << '\n';

				CSS_Anim_ApplyProperties(*newT, gui, css, frame, duration, CSS_Anim_GetEasing(easing));
			}
			/*else
			{
				std::cout << "Ummmmmm... what?" << '\n';
			}*/
		}
		else if (i.find("--move") != std::string::npos)
		{
			if (newT == nullptr)
			{
				newT = Animation::New(70);
				frame = 0;
				duration = 1;
			}

			std::string get = StringAPI::GetSubstringBetween(i, "--move(", ")");
			std::vector<std::string> v = StringAPI::SplitIntoVector(get, " ");

			if (frame - 1 > 0)
				newT->AddTrigger([&] { gui.SetCurrentPosToAlignPivot(); }, frame - 1, 1);

			newT->AddTrigger(gui.localScreenPosition.x, std::stof(v[0]) / 100.0f, frame, duration, CSS_Anim_GetEasing(easing));
			newT->AddTrigger(gui.localScreenPosition.y, std::stof(v[1]) / 100.0f, frame, duration, CSS_Anim_GetEasing(easing));
		}
		else if (i.find("--disable()") != std::string::npos)
			newT->AddTrigger([&] { gui.isEnabled = false; }, frame, 1);
		else if (i.find("--enable()") != std::string::npos)
			newT->AddTrigger([&] { gui.isEnabled = true; }, frame, 1);
	}

	if (type == "onhover")
		gui.onHoverAnim = newT;

	if (type == "onhoverup")
		gui.onHoverOutAnim = newT;

	if (type != "onhover" || type != "onhoverup")
		gui.AddBroadcastAnimation(newT, type);

	if(playOnStart)
		newT->Play();
}

void GUIML::CSS_Anim_ApplyProperties(Animation::Timeline& t, ImGUIElement& gui, std::pair<std::string, std::string> css, int frame, int duration, Animation::Easing e)
{
	css.second = StringAPI::RemoveAll(css.second, "= ");

	if (css.first == "width")
		CSS_Anim_Size(t, gui, "width", css.second, frame, duration, e);
	else if (css.first == "height")
		CSS_Anim_Size(t, gui, "height", css.second, frame, duration, e);

	else if (css.first == "color" || css.first == "colour")
	{
		css.second = StringAPI::ReplaceAll(css.second, "[", "(");
		css.second = StringAPI::ReplaceAll(css.second, "]", ")");
		CSS_Anim_Color(t, gui, RGBToCol(css.second), frame, duration, e);
	}
	else if (css.first == "background-color" || css.first == "background-colour")
	{
		css.second = StringAPI::ReplaceAll(css.second, "[", "(");
		css.second = StringAPI::ReplaceAll(css.second, "]", ")");
		CSS_Anim_BackgroundColor(t, gui, RGBToCol(css.second), frame, duration, e);
	}
	else if (css.first == "align")
		CSS_Anim_Align(t, gui, css.second, frame, duration, e);
}

Animation::Easing GUIML::CSS_Anim_GetEasing(std::string easing)
{
	easing = StringAPI::RemoveAll(easing, " ");

	if (easing == "Linear")
		return Animation::Easing::Linear;

	else if (easing == "Back")
		return Animation::Easing::Back;
	else if (easing == "BackIn")
		return Animation::Easing::BackIn;
	else if (easing == "BackOut")
		return Animation::Easing::BackOut;

	else if (easing == "Bounce")
		return Animation::Easing::Bounce;
	else if (easing == "BounceIn")
		return Animation::Easing::BounceIn;
	else if (easing == "BounceOut")
		return Animation::Easing::BounceOut;

	else if (easing == "Circular")
		return Animation::Easing::Circular;
	else if (easing == "CircularIn")
		return Animation::Easing::CircularIn;
	else if (easing == "CircularOut")
		return Animation::Easing::CircularOut;

	else if (easing == "Cubic")
		return Animation::Easing::Cubic;
	else if (easing == "CubicIn")
		return Animation::Easing::CubicIn;
	else if (easing == "CubicOut")
		return Animation::Easing::CubicOut;

	else if (easing == "Elastic")
		return Animation::Easing::Elastic;
	else if (easing == "ElasticIn")
		return Animation::Easing::ElasticIn;
	else if (easing == "ElasticOut")
		return Animation::Easing::ElasticOut;

	else if (easing == "Exponential")
		return Animation::Easing::Exponential;
	else if (easing == "ExponentialIn")
		return Animation::Easing::ExponentialIn;
	else if (easing == "ExponentialOut")
		return Animation::Easing::ExponentialOut;

	else if (easing == "Quartic")
		return Animation::Easing::Quartic;
	else if (easing == "QuarticIn")
		return Animation::Easing::QuarticIn;
	else if (easing == "QuarticOut")
		return Animation::Easing::QuarticOut;

	else if (easing == "Quintic")
		return Animation::Easing::Quintic;
	else if (easing == "QuinticIn")
		return Animation::Easing::QuinticIn;
	else if (easing == "QuinticOut")
		return Animation::Easing::QuinticOut;

	else if (easing == "Sinusoidal")
		return Animation::Easing::Sinusoidal;
	else if (easing == "SinusoidalIn")
		return Animation::Easing::SinusoidalIn;
	else if (easing == "SinusoidalOut")
		return Animation::Easing::SinusoidalOut;

	return Animation::Easing::Linear;
}

void GUIML::CSS_Anim_Size(Animation::Timeline& t, ImGUIElement& gui, std::string widthOrHeight, std::string v, int frame, int duration, Animation::Easing e)
{
	if (widthOrHeight == "width")
	{
		if (v.find("px") != std::string::npos)
		{
			float pixels = PixelsToFloat(v);

			t.AddTrigger(gui.size.x, pixels, frame, duration, e);
		}
		else if(v.find("vw") != std::string::npos || v.find("%") != std::string::npos)
		{
			v = StringAPI::RemoveAll(v, "vw");
			v = StringAPI::RemoveAll(v, "%");

			float size = std::stof(v);

			if (frame - 1 > 0)
				t.AddTrigger([&] { gui.SetCurrentSizeToScreenSize(true, false); }, frame - 1, 0);

			t.AddTrigger(gui.screenSize.x, size, frame, duration, e);
		}
	}
	else if (widthOrHeight == "height")
	{
		if (v.find("px") != std::string::npos)
		{
			float pixels = PixelsToFloat(v);

			t.AddTrigger(gui.size.y, pixels, frame, duration, e);
		}
		else if (v.find("vh") != std::string::npos || v.find("%") != std::string::npos)
		{
			v = StringAPI::RemoveAll(v, "vh");
			v = StringAPI::RemoveAll(v, "%");

			float size = std::stof(v);

			if(frame - 1 > 0)
				t.AddTrigger([&] { gui.SetCurrentSizeToScreenSize(false, true); }, frame - 1, 0);

			t.AddTrigger(gui.screenSize.y, size, frame, duration, e);
		}
	}
}

void GUIML::CSS_Anim_Color(Animation::Timeline& t, ImGUIElement& gui, Color col, int frame, int duration, Animation::Easing e)
{
	if (!gui.IsColorRefUsedForBackground())
	{
		if (gui.colorRef != nullptr)
		{
			t.AddTrigger(gui.colorRef->r, col.r, frame, duration, e);
			t.AddTrigger(gui.colorRef->g, col.g, frame, duration, e);
			t.AddTrigger(gui.colorRef->b, col.b, frame, duration, e);
			t.AddTrigger(gui.colorRef->a, col.a, frame, duration, e);
		}
	}
	else
	{
		t.AddTrigger(gui.fontColor.r, col.r, frame, duration, e);
		t.AddTrigger(gui.fontColor.g, col.g, frame, duration, e);
		t.AddTrigger(gui.fontColor.b, col.b, frame, duration, e);
		t.AddTrigger(gui.fontColor.a, col.a, frame, duration, e);
	}
}

void GUIML::CSS_Anim_BackgroundColor(Animation::Timeline& t, ImGUIElement& gui, Color col, int frame, int duration, Animation::Easing e)
{
	if (gui.colorRef != nullptr)
	{
		t.AddTrigger(gui.colorRef->r, col.r, frame, duration, e);
		t.AddTrigger(gui.colorRef->g, col.g, frame, duration, e);
		t.AddTrigger(gui.colorRef->b, col.b, frame, duration, e);
		t.AddTrigger(gui.colorRef->a, col.a, frame, duration, e);
	}
}

void GUIML::CSS_Anim_Align(Animation::Timeline& t, ImGUIElement& gui, std::string v, int frame, int duration, Animation::Easing e)
{
	v = StringAPI::RemoveAll(v, "%");
	std::vector<std::string> alignT = StringAPI::SplitIntoVector(v, " ");
	Vector2 align = Vector2(std::stof(alignT[0]), std::stof(alignT[1]));
	align.x /= 100.0f;
	align.y /= 100.0f;

	std::cout << "Align Value: " << align.x << " " << align.y << std::endl;

	if (frame - 1 > 0)
		t.AddTrigger([&] { gui.ForceAlignToCustom(); }, frame - 1, 1);

	t.AddTrigger(gui.alignPivot.x, align.x, frame, duration, e);
	t.AddTrigger(gui.alignPivot.y, align.y, frame, duration, e);
}

std::vector<std::pair<Color, float>> GUIML::TextToGradient(std::string content)
{
	std::string valuesInString = StringAPI::GetSubstringBetween(content, "linear-gradient(", ")!");
	std::vector<std::string> allValues = StringAPI::SplitIntoVector(content, "%,");
	std::vector<std::pair<Color, float>> v;

	for (auto i : allValues)
	{
		// TODO: Add Degree System
		if (i.find("deg") == std::string::npos)
		{
			i += "%";
			std::string percentage = StringAPI::GetSubstringBetween(i, ") ", "%");
			float finalPercentage = std::stof(percentage);

			std::string percentageText = " ";
			percentageText += percentage;
			percentageText += "%";

			std::string removePercentageFromCS = StringAPI::RemoveAll(i, percentageText);
			std::string colorString = StringAPI::RemoveAll(removePercentageFromCS, "linear-gradient(");

			Color finalColor = RGBToCol(colorString);
			v.push_back(std::make_pair(finalColor, finalPercentage));
		}
	}

	return v;
}

float GUIML::PixelsToFloat(std::string val)
{
	if (val.find("px") != std::string::npos)
	{
		std::string v = StringAPI::ReplaceAll(val, "px", "");
		int pix = std::stoi(v);
		return pix;
	}

	return 0.0f;
}

bool GUIML::TextToBool(std::string val)
{
	if (val == "true")
		return true;
	else if (val == "false")
		return false;

	return false;
}
