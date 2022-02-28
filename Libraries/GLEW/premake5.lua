project "GLEW"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	files
	{
		"GLEWCore/src/glew.c",
		"GLEWCore/include/GL/glew.h"
	}

	includedirs
	{
		"GLEWCore/include"
	}

	defines
	{
		"GLEW_STATIC"
	}

	filter "system:windows"
		systemversion "latest"

		files
		{
			"GLEWCore/include/GL/wglew.h"
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
