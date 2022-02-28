#pragma once

// https://repl.it/@CSharpIsGud/IodineCSS

#include <vector>
#include <string>
#include <map>

#include "CSSErrors.h"

using std::string;
using std::map;
using std::vector;

enum class SelectorType {
	Tag,
	Class,
	Id
};

class CSSSelector {
public:
	SelectorType Type;

	string Subject;
};

class CSSStyle {
public:
	string Type = "Color";

	string Text;

	CSSStyle(string text) {
		Text = text;
	}
};

class CSSToken {
public:
	CSSSelector Selector;
	map<string, CSSStyle> Styles = map<string, CSSStyle>();

	CSSToken(CSSSelector selector) {
		Selector = selector;
	}

	CSSToken(CSSSelector selector, map<string, CSSStyle> styles) {
		Selector = selector;
		Styles = styles;
	}
};

// Lexer + Parser
class CSSParser {
public:

	ErrorHolder* Log;

	string Source;

	int Position = 0;

	ErrorHolder Err = ErrorHolder();

	int Line = 1;
	int Column = 1;

	int LastLine;
	int LastColumn;
	int LastPosition;

	char Get() {
		if (Position >= Source.length())
			return '\0';

		Column++;

		if (Source[Position] == '\n') {
			Line++;
			Column = 1;
		}

		return Source[Position++];
	}

	char Peek() {
		if (Position >= Source.length())
			return '\0';

		if (Source[Position] < 0)
			return '\0';

		return Source[Position];
	}

	bool AtEnd() {
		return Position >= Source.length() || Peek() == '\0';
	}

	bool IsSpace(char c) {
		return c == ' ' || c == '\n' || c == '\t';
	}

	bool IsAlpha(char c) {
		return (c >= 'a' && c <= 'z') || 
			(c >= 'A' && c <= 'Z') || 
			(c >= '0' && c <= '9') || 
			c == '-' || 
			c == '%' || 
			c == '\"' || 
			c == '@' || 
			c == '/' || 
			c == '.' ||
			c == '(' ||
			c == ')' ||
			c == ',' ||
			c == '?' ||
			c == '=' ||
			c == '|' ||
			c == '[' ||
			c == ']';
	}

	void GetSpace() {
		while (IsSpace(Peek())) {
			Get();
		}
	}

	string PositionStr() {
		return std::to_string(Line) + ":" + std::to_string(Column);
	}

	string GetIdent() {
		string text = "";

		while (IsAlpha(Peek())) {
			text += Get();
		}

		return text;
	}

	CSSSelector GetSelector() {
		CSSSelector sel = CSSSelector();

		if (Peek() == '#') {
			sel.Type = SelectorType::Id;
			Get();

			sel.Subject = GetIdent();

			return sel;
		}

		if (Peek() == '.') {
			sel.Type = SelectorType::Class;
			Get();

			sel.Subject = GetIdent();

			return sel;
		}

		sel.Type = SelectorType::Tag;

		sel.Subject = GetIdent();

		return sel;
	}

	map<string, CSSStyle> GetBody() {
		GetSpace();

		if (Peek() != '{') {
			Err.Error("Expected '{'");

			Err.Print();

			throw std::runtime_error("Fatal");
			// return map<string, CSSStyle>();
		}

		Get();

		GetSpace();

		map<string, CSSStyle> styles = map<string, CSSStyle>();

		while (Peek() != '}') {
			string key = GetIdent();

			GetSpace(); // "key> <: value"

			if (Peek() != ':') {
				Err.Warn("Missing ':', Ignoring trailing styles");
				break;
			}

			Get();

			GetSpace(); // Ignore whitespace after ':', "key:> <value"

			styles.emplace(key, CSSStyle(GetIdent()));

			GetSpace();

			if (Peek() != ';') {
				Err.Warn("Missing ';', Ignoring trailing styles");
				break;
			}

			Get();

			GetSpace();

			if (Peek() == '\0') {
				Err.Error("Expected '}', unable to parse CSS.");
				break;
			}
		}

		Get();

		return styles;
	}

	CSSToken GetCSSToken() {
		LastPosition = Position;
		LastLine = Line;
		LastColumn = Column;

		GetSpace();

		if (Peek() == '\0')
			return CSSToken(CSSSelector());

		CSSSelector ruleselector = GetSelector();

		return CSSToken(ruleselector, GetBody());
	}

	void Revert() {
		Position = LastPosition;
		Line = LastLine;
		Column = LastColumn;
	}

	CSSParser(string src) {
		Source = src;
	}
};