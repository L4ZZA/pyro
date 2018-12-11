-- workspace is the solution
workspace "Pyro"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release", -- more like a faster debug
		"Dist" -- distribution build
	}

-- example: debug-win-x64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- engine core project
project "Pyro"
	-- location makes sure that everything below will be relative to the project directory
	location "Pyro"
	kind "SharedLib" -- dynamic library (.dll)
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("inter/" .. outputdir .. "/%{prj.name}")

	files
	{
		-- ** means recursively search down that folder
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/external/spdlog/include"
	}

	-- filters are used to apply property to some specific configurations only
	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest" -- windows SDK version

		defines
		{
			"PYRO_PLATFORM_WIN",
			"PYRO_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "PYRO_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PYRO_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PYRO_DIST"
		optimize "On"

-- sandbox application
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("inter/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Pyro/external/spdlog/include",
		"Pyro/src"
	}

	links
	{
		"Pyro"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PYRO_PLATFORM_WIN"
		}

	filter "configurations:Debug"
		defines "PYRO_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "PYRO_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "PYRO_DIST"
		optimize "On"