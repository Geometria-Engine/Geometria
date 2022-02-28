project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	files
	{
		"GLFWCore/include/GLFW/glfw3.h",
		"GLFWCore/include/GLFW/glfw3native.h",
		"GLFWCore/src/glfw_config.h",
		"GLFWCore/src/context.c",
		"GLFWCore/src/init.c",
		"GLFWCore/src/input.c",
		"GLFWCore/src/monitor.c",
		"GLFWCore/src/vulkan.c",
		"GLFWCore/src/window.c"
	}

	filter "system:linux"
		systemversion "latest"

		files
		{
			"GLFWCore/src/x11_init.c",
			"GLFWCore/src/x11_monitor.c",
			"GLFWCore/src/x11_window.c",
			"GLFWCore/src/xkb_unicode.c",
			"GLFWCore/src/posix_time.c",
			"GLFWCore/src/posix_thread.c",
			"GLFWCore/src/glx_context.c",
			"GLFWCore/src/egl_context.c",
			"GLFWCore/src/osmesa_context.c",
			"GLFWCore/src/linux_joystick.c"
		}

		defines
		{
			"_GLFW_X11"
		}

	filter "system:windows"
		systemversion "latest"

		files
		{
			"GLFWCore/src/win32_init.c",
			"GLFWCore/src/win32_joystick.c",
			"GLFWCore/src/win32_monitor.c",
			"GLFWCore/src/win32_time.c",
			"GLFWCore/src/win32_thread.c",
			"GLFWCore/src/win32_window.c",
			"GLFWCore/src/wgl_context.c",
			"GLFWCore/src/egl_context.c",
			"GLFWCore/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

		links
		{
			"Dwmapi.lib"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
