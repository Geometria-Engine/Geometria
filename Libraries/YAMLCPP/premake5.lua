project "yaml-cpp"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	includedirs
	{
		"YAMLCPPCore/include"
	}

	files
	{
		"YAMLCPPCore/src/**.h",
		"YAMLCPPCore/src/**.cpp",
		
		"YAMLCPPCore/include/**.h"
	}

	filter "system:windows"
		systemversion "latest"

	filter "system:linux"
		systemversion "latest"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"