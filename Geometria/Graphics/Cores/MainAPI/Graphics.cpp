#include <iostream>
#include "Graphics.h"

//==[STATIC VARIABLES]==//

bool Graphics::_init, Graphics::_close, Graphics::_bypassIntel;
Window Graphics::_currentWindow;
int Window::width;
int Window::height;
float Graphics::_lastDeltaTime;
int Graphics::_oGLDepthFunction;

//======================//

void Graphics::Init()
{
	//The beginning of the Graphics API.
	//Check and initialize it here.

	//==[ OPENGL ]==//
	{
		_init = glfwInit();
	}
}

Window Graphics::CreateWindow()
{
	//We don't need to copy paste the code again, so set these values by default.
	return Graphics::CreateWindow(1280, 720, "Geometria Engine");
}

void Graphics::Start()
{

}

void Graphics::Update()
{
	//Update the delta time
	UpdateDeltaTime();

	//==[ OPENGL ]==//
		//Start rendering here
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//And end it with basic OpenGL stuff, you poll events and swap buffers in order to get the update function going.
	glfwPollEvents();

	glfwSwapBuffers(_currentWindow.openGLWindow);

	glfwSwapInterval(1);
}

bool Graphics::CanClose()
{
	//==[ OPENGL ]==//
	return glfwWindowShouldClose(_currentWindow.openGLWindow) || Graphics::_close;
}

bool Graphics::Exit()
{
	//Exit and terminate stuff here properly in order to prevent any glitches or bugs

	//==[ OPENGL ]==//
	glfwDestroyWindow(_currentWindow.openGLWindow);
	glfwTerminate();
	Graphics::_close = true;
	_currentWindow.openGLWindow == nullptr;
	return true;
}

Window Graphics::CreateWindow(int width, int height, const char* title)
{
	//This is the class that creates all the graphical setup for the Window class.

	//Just to prevent any errors, we're going to check if the graphics are working correctly first.
	if (Graphics::IsWorking())
	{
		//If this is actually working correctly, then we can create the Window that we'll use temporarly.
		Window win;

		//Start saving extra info (resolution, title, etc.).
		win.width = width;
		win.height = height;

		win.Title = title;

		//After that, in order to sync it with our graphics API, create the "true window".
		_currentWindow = win;

		//==[ OPENGL ]==//
		_currentWindow.openGLWindow = glfwCreateWindow(_currentWindow.width, _currentWindow.height, _currentWindow.Title, NULL, NULL);
		//Make this window into the current context.

		glfwMakeContextCurrent(_currentWindow.openGLWindow);

		//And set the window user pointer so it knows that object we're combining with OpenGL.
		glfwSetWindowUserPointer(_currentWindow.openGLWindow, &_currentWindow);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "GLEW not initialized!" << std::endl;
			return Window();
		}

		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		//glEnable(GL_FRAMEBUFFER_SRGB);

		//And set all library callbacks (keyboard, mouse, etc.).
		SetAllCallbacks();

		//Once we're done, we can return it safely :D.
		return _currentWindow;
	}

	//This returns empty data in case something fails.
	return Window();
}

void Graphics::SetAllCallbacks()
{
	//We set everything all that the Graphic Libraries can return.
	//In other Library APIs there's probably no need
	//Or they set these stuff automatically for you, but just in case here it is.
	std::cout << "Setting callbacks!" << std::endl;

	SetKeyboard();
	SetMouse();
	SetCursor();
	GetFrameBuffer();
	SetResizeCall();
}

void Graphics::SetKeyboard()
{
	//==[ OPENGL ]==//
	{
		glfwSetKeyCallback(_currentWindow.openGLWindow, _currentWindow.SetKeyboard);
	}
}

void Graphics::SetMouse()
{
	//==[ OPENGL ]==//
	{
		glfwSetMouseButtonCallback(_currentWindow.openGLWindow, _currentWindow.SetMouse);
	}
}

void Graphics::SetCursor()
{
	//==[ OPENGL ]==/
	{
		glfwSetCursorPosCallback(_currentWindow.openGLWindow, _currentWindow.SetCursor);
	}
}

void Graphics::GetFrameBuffer()
{
	//==[ OPENGL ]==//
	{
		glfwGetFramebufferSize(_currentWindow.openGLWindow, &_currentWindow.width, &_currentWindow.height);
	}
}

void Graphics::SetResizeCall()
{
	//==[ OPENGL ]==//
	{
		glfwSetWindowSizeCallback(_currentWindow.openGLWindow, _currentWindow.ResizeWindow);
		glfwSetFramebufferSizeCallback(_currentWindow.openGLWindow, _currentWindow.ResizeWindow);
	}
}


//=================================[ GPU ]=================================//

std::string Graphics::ShowGraphicsCard()
{
	const unsigned char* rend = glGetString(GL_RENDERER);
	return std::string((char*)rend);
}

bool Graphics::IsIntelGPU()
{
	std::cout << "[GPU] " << Graphics::ShowGraphicsCard() << std::endl;
	return Graphics::ShowGraphicsCard().find("Intel") != std::string::npos;
}

bool Graphics::IsAMDGPU()
{
	std::cout << "[GPU] " << Graphics::ShowGraphicsCard() << std::endl;
	return Graphics::ShowGraphicsCard().find("Radeon") != std::string::npos || Graphics::ShowGraphicsCard().find("AMD") != std::string::npos;
}

void Graphics::GPU_CreateBuffer(uint32_t& bufferObject, int bufferType, int size, void* data)
{

}

void Graphics::GPU_CreateBuffers(uint32_t& vertexArrayObject, uint32_t& vertexBuffer, uint32_t& indexBuffer, float* vertices)
{
	//==[ OPENGL ]==//
	{
		glGenVertexArrays(1, &vertexArrayObject);
		glGenBuffers(1, &vertexBuffer);

		// Bind VAO, then bind VBO
		glBindVertexArray(vertexArrayObject);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		// Copy the vertex data into the buffer's memory
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Set attributes that describe how OpenGL should interpret the vertex data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		/*glGenVertexArrays(1, vertexArrayObject);
		glBindVertexArray(*vertexArrayObject);

		glGenBuffers(1, vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);*/

		/*glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t), nullptr, GL_DYNAMIC_DRAW);*/
	}
}

void Graphics::GPU_AddPointerVector2(int order, int size, void* offset, bool normalized)
{
	//==[ OPENGL ]==//
	{
		glVertexAttribPointer(order, 2, GL_FLOAT, normalized, size, offset);
		glEnableVertexAttribArray(order);
	}
}

void Graphics::GPU_AddPointerVector3(int order, int size, void* offset, bool normalized)
{
	//==[ OPENGL ]==//
	{
		glVertexAttribPointer(order, 3, GL_FLOAT, normalized, size, offset);
		glEnableVertexAttribArray(order);
	}
}

void Graphics::GPU_AddPointerFloat(int order, int size, void* offset, bool normalized)
{
	//==[ OPENGL ]==//
	{
		glVertexAttribPointer(order, 1, GL_FLOAT, normalized, size, offset);
		glEnableVertexAttribArray(order);
	}
}

void Graphics::GPU_AddPointerInt(int order, int size, void* offset, bool normalized)
{
	//==[ OPENGL ]==//
	{
		glVertexAttribPointer(order, 1, GL_INT, normalized, size, offset);
		glEnableVertexAttribArray(order);
	}
}

void Graphics::GPU_FinishBufferCreation()
{
	//==[ OPENGL ]==//
	{
		glBindVertexArray(0);
	}
}

void Graphics::GPU_SetBufferData(int BufferType, int size, void* data)
{
	//==[ OPENGL ]==//
	glBufferData(BufferType, size, data, GL_DYNAMIC_DRAW);
}

void Graphics::GPU_UpdateBufferData(int BufferType, int size, void* data)
{
	//==[ OPENGL ]==//
	glBufferSubData(BufferType, 0, size, data);
}

void Graphics::GPU_DrawElements(int drawMode, int size, int dataType, void* indices)
{
	//==[ OPENGL ]==//
	glDrawElements(drawMode, size, dataType, indices);
}

void Graphics::GPU_DrawBatch(int size)
{
	//==[ OPENGL ]==//
	GPU_DrawElements(GL_TRIANGLES, (GLsizei)size, GL_UNSIGNED_INT, nullptr);
}
