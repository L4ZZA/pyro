-- workspace is the solution
workspace "pyro"
    architecture "x64"
    startproject "sandbox"

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
IncludeDir["GLFW"]      = "pyro/external/GLFW/include"
IncludeDir["Glad"]      = "pyro/external/Glad/include"
IncludeDir["glm"]       = "pyro/external/glm"
IncludeDir["stb_image"] = "pyro/external/stb_image"
IncludeDir["ImGui"]     = "pyro/external/imgui"
IncludeDir["assimp"]    = "pyro/external/assimp/include/"
IncludeDir["assimpcfg"] = "pyro/external/assimp/config/"

group "dependencies"
    include "pyro/external/GLFW"
    include "pyro/external/Glad"
    include "pyro/external/imgui"
    include "pyro/external/stb_image"
    -- include "pyro/external/assimp"
group ""

-- engine core project
project "pyro"
    -- location makes sure that everything below will be relative to the project directory
    location "pyro"
    kind "StaticLib" -- Static library (.lib)
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("inter/" .. outputdir .. "/%{prj.name}")

    pchheader "pyro_pch.h"
    pchsource "pyro/src/pyro_pch.cpp"

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
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/external/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "stb_image",
    }

    -- filters are used to apply property to some specific configurations only
    filter "system:windows"
        systemversion "latest" -- windows SDK version

        defines
        {
            "GLFW_INCLUDE_NONE",
            "PYRO_PLATFORM_WIN",
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
project "sandbox"
    location "sandbox"
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
        "pyro/external/spdlog/include",
        "pyro/src",
        "pyro/external",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
    }

    links
    {
        "pyro"
    }

    filter "system:windows"
        systemversion "latest"

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
