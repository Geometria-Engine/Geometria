#ifndef MINIAUDIOMANAGER_H
#define MINIAUDIOMANAGER_H

#include "geometria.h"
#include "../miniaudio.h"

struct MiniAudioManager : public ScriptBehaviour
{
    static ma_result result;
    static ma_engine engine;
    static bool isEngineInitialized;

    static void StartEngine()
    {
        std::cout << "Starting Mini Audio Engine..." << std::endl;

        result = ma_engine_init(NULL, &engine);

        if (result != MA_SUCCESS) 
        {
            std::cout << "Failed to initialize audio engine." << std::endl;
        }
        else
            isEngineInitialized = true;
    }

    void OnStart()
    {
        StartEngine();
    }
};

#endif