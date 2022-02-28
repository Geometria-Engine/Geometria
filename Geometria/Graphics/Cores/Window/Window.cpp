#include "../MainAPI/Graphics.h"
#include "Window.h"
#include <iostream>

bool Window::mouse[32];
bool Window::keys[1024];
int Window::keysState[1024];

void Window::SetKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//==[ OPENGL ]==//
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->keys[key] = action != GLFW_RELEASE;
	}
}

void Window::SetMouse(GLFWwindow* window, int button, int action, int mods)
{
	//==[ OPENGL ]==//
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->mouse[button] = action != GLFW_RELEASE;
	}
}

bool window_MouseStartUp = true;
void Window::SetCursor(GLFWwindow* window, double xPos, double yPos)
{
	//==[ OPENGL ]==//
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->Mouse.X = xPos;
		win->Mouse.Y = yPos;

		if (window_MouseStartUp)
		{
			win->Mouse.lastX = win->Mouse.X;
			win->Mouse.lastY = win->Mouse.Y;
			window_MouseStartUp = false;
		}

		win->Mouse.speedX = win->Mouse.X - win->Mouse.lastX;
		win->Mouse.speedY = win->Mouse.Y - win->Mouse.lastY;

		if (win->Mouse.speedX < 5 && win->Mouse.speedX > -5)
			win->Mouse.speedX = 0;

		if (win->Mouse.speedY < 5 && win->Mouse.speedY > -5)
			win->Mouse.speedY = 0;

		win->Mouse.lastX = win->Mouse.X;
		win->Mouse.lastY = win->Mouse.Y;
	}
}

void Window::ResizeWindow(GLFWwindow* window, int width, int height)
{
	//==[ OPENGL ]==//
	{
		glfwGetFramebufferSize(window, &Window::width, &Window::height);
		//std::cout << "Changed Width: " << Window::width << " || Changed Height: " << Window::height << std::endl;
		//std::cout << "Current Width: " << Graphics::GetMainWindow().width << " || Current Height: " << Graphics::GetMainWindow().height << std::endl;
		Graphics::MainCamera()->ProjectionMatrix(Graphics::MainCamera()->fov, (float)Window::width / (float)Window::height, Graphics::MainCamera()->nearPlane, Graphics::MainCamera()->farPlane);
		glViewport(0, 0, width, height);
	}
}
