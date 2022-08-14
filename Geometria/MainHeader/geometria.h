#pragma once
#ifndef GEOMETRIA_H
#define GEOMETRIA_H
#include "../Graphics/Cores/MainAPI/Graphics.h"
#include "../Graphics/Cores/Renderer/RendererCore.h"
#include "../Graphics/Externals/SceneAndDrawCall.h"
#include "../Input/Input.h"
#include "../Files/Files.h"
#include "../Graphics/Cores/Texture/Texture.h"
#include "../Compiler/CompilationCore.h"
#include "../CommonObjects/Empty.h"
#include "../WebRequest/WebRequest.h"
#include "Files/GUIML/GUIML.h"
#include "Multithreading/Multithreading.h"
#include "Animation/Animation.h"
#include "Broadcast/Broadcast.h"
#include "DebugTools/DebugTools.h"
#include "String/StringAPI.h"
#include "Web/Web.h"
#include "MiniAudio/MiniAudioSource.h"
#undef CreateWindow
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#endif

// Toolkit Macros
#include "geometria/toolkitmacros.h"