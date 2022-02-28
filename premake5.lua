workspace "Geometria"
	staticruntime "on"
	configurations { "Release" }
	platforms { "x86_64", "x86" }
	startproject "Geometria"

IncludeDirectories = {}

include "Libraries/GLFW"
IncludeDirectories["GLFW"] = "Libraries/GLFW/GLFWCore/include/GLFW"

include "Libraries/GLEW"
IncludeDirectories["GLEW"] = "Libraries/GLEW/GLEWCore/include"

include "Libraries/YAMLCPP"
IncludeDirectories["YAMLCPP"] = "Libraries/YAMLCPP/YAMLCPPCore/include"

IncludeDirectories["NVIDIAPHYSX"] = "Libraries/NVIDIAPHYSX/include"
IncludeDirectories["NVIDIAPHYSXSHARED"] = "Libraries/NVIDIAPHYSX/pxshared/include"

include "Libraries/CURL"
IncludeDirectories["CURL"] = "Libraries/CURL/include"

include "Libraries/ZLIB"
IncludeDirectories["ZLIB"] = "Libraries/ZLIB"


project "Geometria"
	kind "ConsoleApp"
	language "C++"
	location "%{prj.name}"
	targetdir "%{prj.name}"

	files 
	{ 
		"%{prj.name}/**.h", 
		"%{prj.name}/**.hpp", 
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
		"Libraries/include",
		"Geometria/Behaviours",
		"Geometria/MainHeader",
		"Geometria/",
		"Geometria/Game",
		"%{IncludeDirectories.GLFW}",
		"%{IncludeDirectories.GLEW}",
		"%{IncludeDirectories.YAMLCPP}",
		"%{IncludeDirectories.NVIDIAPHYSX}",
		"%{IncludeDirectories.NVIDIAPHYSXSHARED}",
		"%{IncludeDirectories.CURL}",
		"%{IncludeDirectories.ZLIB}"
	}
	
	filter "system:windows"

	   links
	   {
	    	"GLFW",
			"GLEW",
			"yaml-cpp",
			"curl-lib",
			"opengl32.lib",
			"crypt32.lib",
			"wldap32.lib", 
			"Ws2_32.lib",
			"zlib"
	   }

	filter "system:linux"
	   links
	   {
	   		"GLFW",
			"GLEW",
			"yaml-cpp",
			"curl",
			"zlib",
			"GL",
			"pthread",
			"m",
			"dl",
			"stdc++fs",

			-- NVIDIA PhysX Libraries --
			"Libraries/NVIDIAPHYSX/binaries/Linux/PhysX_static_64",
			"Libraries/NVIDIAPHYSX/binaries/Linux/PhysXExtensions_static_64",
			"Libraries/NVIDIAPHYSX/binaries/Linux/PhysXCharacterKinematic_static_64",
			"Libraries/NVIDIAPHYSX/binaries/Linux/PhysXCooking_static_64",
			"Libraries/NVIDIAPHYSX/binaries/Linux/PhysXFoundation_static_64",
			"Libraries/NVIDIAPHYSX/binaries/Linux/PhysXVehicle_static_64",
			"Libraries/NVIDIAPHYSX/binaries/Linux/PhysXPvdSDK_static_64",
			"Libraries/NVIDIAPHYSX/binaries/Linux/PhysXCommon_static_64"
	   }

	filter "configurations:Release"
		defines 
		{ 
			"_SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING",
			"GLEW_STATIC",
			"NDEBUG",
			"_CONSOLE",
			"CURL_STATICLIB"
		}
		optimize "On"

    filter "platforms:x86"
	if(system == "windows") then
		defines
		{
			"WIN32"
		}
	end
        architecture "x86"

    filter "platforms:x86_64"
        architecture "x86_64"

    -- NVIDIA PhysX Libraries (Win64) --
    filter { "system:windows", "platforms:x86" }
    	links
		{
			"Libraries/NVIDIAPHYSX/binaries/Win32/LowLevel_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/LowLevelAABB_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/LowLevelDynamics_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/PhysX_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/PhysXCharacterKinematic_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/PhysXCommon_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/PhysXCooking_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/PhysXExtensions_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/PhysXFoundation_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/PhysXPvdSDK_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/PhysXVehicle_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/SceneQuery_static_32.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win32/SimulationController_static_32.lib"
		}

	-- NVIDIA PhysX Libraries (Win32) --
	filter { "system:windows", "platforms:x86_64" }
		links
		{
			"Libraries/NVIDIAPHYSX/binaries/Win64/LowLevel_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/LowLevelAABB_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/LowLevelDynamics_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/PhysX_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/PhysXCharacterKinematic_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/PhysXCommon_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/PhysXCooking_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/PhysXExtensions_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/PhysXFoundation_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/PhysXPvdSDK_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/PhysXVehicle_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/SceneQuery_static_64.lib",
			"Libraries/NVIDIAPHYSX/binaries/Win64/SimulationController_static_64.lib"
		}

    filter "system:windows"
    	systemversion "latest"
