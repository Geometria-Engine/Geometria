#pragma once

#ifndef BEHAVIOUR_H
#include "Behaviour.h"
#endif

#ifndef GEOMETRIA_H
#include "geometria.h"
#endif

struct AudioSource : public ScriptBehaviour
{
	AllowCloning(AudioSource);

	std::string audioFile = "Resources/StereoMadness.mp3";
	bool isPlaying = false;

	void Play();

	void OnStart();
};