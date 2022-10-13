#ifndef I_IMAGE_H
#define I_IMAGE_H

#include "../iGUI.h"

struct iImage : public iGUI
{
	Texture* _texture = nullptr;

	Vector2 _trueSca;
	Vector2 _truePos;

	void Image_SetParams();

	iImage(iGUI& owner)
	{
		owner.allElements.push_back(this);
		this->owner = &owner;
	}

	iImage(iGUI& owner, Texture* t)
	{
		owner.allElements.push_back(this);
		this->owner = &owner;
		_texture = t;
	}

	void OnUpdate();
};

#endif