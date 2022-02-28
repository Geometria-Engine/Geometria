#include "Application.h"
#include "../Behaviours/Behaviour.h"
#include "../Graphics/Cores/MainAPI/Graphics.h"
#include "../Graphics/Cores/Renderer/RendererCore.h"
#include "../Graphics/Externals/SceneAndDrawCall.h"

//SCENES
//#include "../Game/SampleScene.tits.h"
#include "../Game/GameMain.h"

#include "../Editor/Editor.h"
#include "../Graphics/Cores/Texture/Texture.h"

#include "Multithreading/Multithreading.h"

Application::State Application::_engineState;

bool Application::IsPlatform(Platform p)
{
	Platform currentPlatform;
#ifdef _WIN32
	currentPlatform = Platform::Windows;
#endif

#ifdef __linux__
	currentPlatform = Platform::Linux;
#endif

	return p == currentPlatform;
}

void Application::SetEditor()
{
	Application::_engineState = State::Editor;
	Hierarchy::StartScripts();
	Editor::SwitchEditorView();
}

void Application::SetGame()
{	
	Application::_engineState = State::Game;
	Hierarchy::StartScripts();
	Editor::SwitchGameView();
}

void Application::Start()
{
	TextureManager::ClearRAM();

	RendererCore::EndThreads();

	Multithreading::Start();

	for (int i = 0; i < Hierarchy::allScripts.size(); i++)
	{
		if (Hierarchy::allScripts[i] != nullptr)
			Hierarchy::allScripts[i]->DestroyScript();

		Hierarchy::allScripts[i] = nullptr;
	}

	Hierarchy::allStaticScripts.clear();
	std::vector<ScriptBehaviour*>().swap(Hierarchy::allStaticScripts);

	Hierarchy::listOfStaticScripts.clear();
	std::vector<std::string>().swap(Hierarchy::listOfStaticScripts);

	Hierarchy::allScripts.clear();
	std::vector<ScriptBehaviour*>().swap(Hierarchy::allScripts);

	Graphics::MainCamera() = new Camera(Vector3(0, 0, 2), 70.0f, (float)Graphics::GetMainWindow().width / (float)Graphics::GetMainWindow().height, 0.1f, 1000.0f);
	Graphics::MainCamera()->objectClassName = "Main Camera";

	RendererCore::SetUp();

	Application::SetEditor();

	RendererCore::Start();

	GameMain::Init();

	Application::SetGame();
}
