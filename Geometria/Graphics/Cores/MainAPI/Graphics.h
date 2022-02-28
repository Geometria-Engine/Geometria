#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <vector>
#include "../Window/Window.h"
#include "../Shader/Shader.h"
#include "../Camera/Camera.h"

class RendererCore;
class Camera;

//#include "../../Externals/Model.h" << INCLUDED IN SHADER.H

class Graphics
{
public:
	//==[INIT]==/
	static void Init();
	static Window CreateWindow();
	static Window CreateWindow(int width, int height, const char* title);
	//==[INIT]==/

	//==[MAIN FUNCTIONS]==/
	static void Start();
	static void Update();

	static bool CanClose();
	static bool Exit();
	//==[MAIN FUNCTIONS]==/

	//==[GRAPHIC LIBRARY CALLBACKS]==/
	static void SetAllCallbacks();

	static void	SetKeyboard();
	static void SetMouse();
	static void SetCursor();
	static void GetFrameBuffer();
	static void SetResizeCall();
	//==[GRAPHIC LIBRARY CALLBACKS]==/

	//=================================[ GPU ]=================================//
	static std::string ShowGraphicsCard();
	static bool IsIntelGPU();
	static bool IsAMDGPU();

	static void GPU_CreateBuffer(uint32_t& bufferObject, int bufferType, int size, void* data);
	static void GPU_CreateBuffers(uint32_t& vertexArrayObject, uint32_t& vertexBuffer, uint32_t& indexBuffer, float* vertices);

	static void GPU_AddPointerVector2(int order, int size, void* offset, bool normalized);
	static void GPU_AddPointerVector3(int order, int size, void* offset, bool normalized);
	static void GPU_AddPointerFloat(int order, int size, void* offset, bool normalized);
	static void GPU_AddPointerInt(int order, int size, void* offset, bool normalized);

	static void GPU_FinishBufferCreation();

	static void GPU_SetBufferData(int BufferType, int size, void* data);
	static void GPU_UpdateBufferData(int BufferType, int size, void* data);

	static void GPU_DrawElements(int drawMode, int size, int dataType, void* indices);
	static void GPU_DrawBatch(int size);
	//=================================[ GPU ]=================================//

private:
	static Window _currentWindow;
	static bool _init, _close;
	static float _deltaTime;
	static float _lastDeltaTime;
	static Camera* _mainCamera;
	static int _oGLDepthFunction;
	static Vector2 _mousePosition, _mouseSpeed;
	static bool _bypassIntel;
public:
	static void BypassIntel(bool t)
	{
		_bypassIntel = t;
	}

	static bool IsIntelGPUBypassed()
	{
		return _bypassIntel;
	}

	static bool IsWorking()
	{
		return _init;
	}

	static int& OpenGLDepthFunction()
	{
		return _oGLDepthFunction;
	}

	static Window GetMainWindow()
	{
		return _currentWindow;
	}

	static float DeltaTime()
	{
		return _deltaTime;
	}

	static Camera*& MainCamera()
	{
		return _mainCamera;
	}

	static void UpdateDeltaTime()
	{
		//==[ OPENGL ]==//
		{
			float openGLDeltaTime = static_cast<float>(glfwGetTime());
			_deltaTime = openGLDeltaTime - _lastDeltaTime;
			_lastDeltaTime = openGLDeltaTime;
		}

	}

	static Vector2 MouseSpeed()
	{
		Vector2 result(GetMainWindow().Mouse.speedX, GetMainWindow().Mouse.speedY);
		return result;
	}

	static Vector2 MousePosition()
	{
		Vector2 result(GetMainWindow().Mouse.X, GetMainWindow().Mouse.Y);
		return result;
	}
};
