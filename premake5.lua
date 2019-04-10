-- workspace is the solution
workspace "Pyro"
    architecture "x64"
    startproject "Sandbox"

    configurations
    {
        "Debug",
        "Release", -- more like a faster debug
        "Dist" -- distribution build
    }

-- example: debug-win-x64
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- include directories relative to root folder (sln dir)
IncludeDir = {}
IncludeDir["GLFW"] = "Pyro/external/GLFW/include/"
IncludeDir["Glad"] = "Pyro/external/Glad/include/"
IncludeDir["ImGui"] = "Pyro/external/imgui/"
IncludeDir["glm"] = "Pyro/external/glm/"

-- iclude GLFW premake file
include "Pyro/external/GLFW/"
include "Pyro/external/Glad/"
include "Pyro/external/ImGui/"

-- engine core project
project "Pyro"
    -- location makes sure that everything below will be relative to the project directory
    location "Pyro"
    kind "StaticLib" -- Static library (.lib)
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("inter/" .. outputdir .. "/%{prj.name}")
    
    pchheader "pyro_pch.h"
    pchsource "Pyro/src/pyro_pch.cpp"

    files
    {
        -- ** means recursively search down that folder
        "%{prj.name}/src/**.h", 
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/external/glm/glm/**.hpp", 
        "%{prj.name}/external/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/external/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib",
        "ImGui"
    }

    -- filters are used to apply property to some specific configurations only
    filter "system:windows"
        systemversion "latest" -- windows SDK version

        defines
        {
            "PYRO_PLATFORM_WIN",
            "PYRO_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
        }

    filter "configurations:Debug"
        defines "PYRO_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "PYRO_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "PYRO_DIST"
        runtime "Release"
        optimize "on"

-- sandbox application
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "Pyro/src",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
    }

    links
    {
        "Pyro",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "PYRO_PLATFORM_WIN",
            "IMGUI_API=__declspec(dllimport)"
        }

    filter "configurations:Debug"
        defines "PYRO_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "PYRO_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "PYRO_DIST"
        runtime "Release"
        optimize "on"