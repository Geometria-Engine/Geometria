#pragma once
#include <glfw3.h>

class Graphics;

class Window
{
public:
	//==[OPENGL]==//
	GLFWwindow* openGLWindow;
	//============//

	//==[INPUTS AND CALLBACKS MANAGER]==//

	//Warning: Most voids will have functions used in the "Graphics" class.
	static bool keys[1024];
	static int keysState[1024];
	static bool mouse[32];

	static bool SizeChanged;

	//==[ OPENGL ]==//
	static void SetKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void SetMouse(GLFWwindow* window, int button, int action, int mods);
	static void SetCursor(GLFWwindow* window, double xPos, double yPos);
	static void ResizeWindow(GLFWwindow* window, int width, int height);
	//==============//

	//==================================//

	//The Window Class doesn't handle graphics, this one only stores data.
	//The one that actually does that job is "Graphics.cpp/.h".

	Window()
	{

	}

	static int width;
	static int height;

	struct
	{
		double X;
		double Y;
		double speedX;
		double speedY;
		double lastX;
		double lastY;
	} Mouse;

	const char* Title;

};
