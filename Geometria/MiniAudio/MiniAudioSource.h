#ifndef MINIAUDIOSOURCE_H
#define MINIAUDIOSOURCE_H

#include "geometria.h"
#include "miniaudio.h"

struct MiniAudioSource : public ScriptBehaviour
{
	AllowCloning(MiniAudioSource);

	ma_sound internalSound;

	std::string audioFile;
	bool isPlaying = false;
	bool playOnStart = false;
	bool setAudioLoadingOnUpdate = false;

	void LoadAudioFile(std::string file);
	void Play();
	void PlayOnStart();
	void Stop();
	void SetTime(float milliseconds);

	void OnStartup();
	void OnUpdate();
};

#endif