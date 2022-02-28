#pragma once

#include "geometria.h"

#ifndef MINIAUDIO_IMPLEMENTATION
//#define MA_DEBUG_OUTPUT
#define MINIAUDIO_IMPLEMENTATION
#include "../miniaudio.h"
#endif

struct AudioManager
{
    static ma_result result;
    static ma_decoder decoder;
    static ma_device_config deviceConfig;
    static ma_device device;
};