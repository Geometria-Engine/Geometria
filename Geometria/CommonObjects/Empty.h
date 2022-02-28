#pragma once

#ifndef BEHAVIOUR_H
#include "Behaviour.h"
#endif

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

struct Empty : public ScriptBehaviour
{
	// This is the only thing that it needs, the engine will take care of the rest.

	void OnStartup()
	{
		ClassType = Class::Object;
	}
};
