#include "DragBorderlessWindow.h"
#include "Graphics/Cores/MainAPI/Graphics.h"

void DragBorderlessWindow::OnStart()
{
	std::cout << "Borderless Drag Added!" << std::endl;
}

void DragBorderlessWindow::OnUpdate()
{
	if (Input::GetMouseButton(0) && Graphics::MousePosition().y < 100)
	{
		double xpos, ypos;
		glfwGetCursorPos(Graphics::GetMainWindow().openGLWindow, &xpos, &ypos);

		int win_x, win_y, delta_x, delta_y;

		if (!is_clicked)
		{
			mouseClick = Graphics::MousePosition();
			is_clicked = true;
		}
		
		delta_x = xpos - mouseClick.x;
		delta_y = ypos - mouseClick.y;

		glfwGetWindowPos(Graphics::GetMainWindow().openGLWindow, &win_x, &win_y);
		glfwSetWindowPos(Graphics::GetMainWindow().openGLWindow, 
			win_x + delta_x, 
			win_y + delta_y);
	}
	else
		is_clicked = false;
}
