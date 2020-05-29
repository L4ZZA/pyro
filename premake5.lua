
require('vstudio')

local vs = premake.vstudio.vc2010

local function premakeVersionComment(prj)
    premake.w('<!-- Generated by Premake ' .. _PREMAKE_VERSION .. ' -->')
end

local function vcpkg(prj)
    if prj.name == 'tests' then 
        printf("Appended to '%s' project", prj.name)
        premake.w('<VcpkgTriplet>x64-windows-static</VcpkgTriplet>')
        premake.w('<VcpkgEnabled>true</VcpkgEnabled>')
    end
end

premake.override(premake.vstudio.vc2010.elements, "project", function(base, prj)
    local calls = base(prj)
    table.insertafter(calls, vs.xmlDeclaration, premakeVersionComment)
    return calls
end)

premake.override(premake.vstudio.vc2010.elements, "globals", function(base, prj)
    local calls = base(prj)
    table.insertafter(calls, vs.globals, vcpkg)
    return calls
end)

-- workspace is the solution
workspace "pyro"
    architecture "x64"
    startproject "level_editor"

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
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
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

-- level_editor application
project "level_editor"
    location "level_editor"
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
        "%{prj.name}/src/",
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

-- include "tests"
-- Test application
project "tests"
    location "tests"

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
        "%{prj.name}/src/",
        "level_editor/src",
        "pyro/external/spdlog/include",
        "pyro/src",
        "pyro/external",
        "%{IncludeDir.glm}",
        "%{IncludeDir.ImGui}",
    }

    links
    {
        "level_editor",
        "pyro",
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "NOMINMAX",
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
