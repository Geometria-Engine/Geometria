#ifndef I_VALUE_H
#define I_VALUE_H

#include "../iGUI.h"

struct iValue : public iGUI
{
	std::string title = "";

	int* _intP = nullptr;
	float* _floatP = nullptr;
	double* _doubleP = nullptr;

	float speed = 0.1f;

	Vector2* _vec2P = nullptr;
	Vector3* _vec3P = nullptr;
	Vector4* _vec4P = nullptr;

	iValue(iGUI& owner, std::string title, int* v)
	{
		owner.allElements.push_back(this);
		this->owner = &owner;
		this->title = title;
		_intP = v;
	}

	iValue(iGUI& owner, std::string title, float* v)
	{
		owner.allElements.push_back(this);
		this->owner = &owner;
		this->title = title;
		_floatP = v;
	}

	iValue(iGUI& owner, std::string title, Vector3* v)
	{
		owner.allElements.push_back(this);
		this->owner = &owner;
		this->title = title;
		_vec3P = v;
	}

	void OnUpdate();
};

#endif