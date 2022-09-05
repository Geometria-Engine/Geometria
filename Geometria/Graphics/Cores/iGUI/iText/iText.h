#ifndef I_TEXT_H
#define I_TEXT_H

#include "../iGUI.h"

struct iText : public iGUI
{
	std::string text = "";

	iText(iGUI& owner, std::string text)
	{
		owner.allElements.push_back(this);
		this->owner = &owner;
		this->text = text;
	}

	void OnUpdate();
};

#endif