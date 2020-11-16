
project "pyro"

kind "StaticLib" -- Static library (.lib)
language "C++"
cppdialect "C++17"
staticruntime "on"

targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir ("%{wks.location}/inter/" .. outputdir .. "/%{prj.name}")

pchheader "pyro_pch.h"
pchsource "src/pyro_pch.cpp"

files
{
    -- ** means recursively search down that folder
    "src/**.h",
    "src/**.cpp",
    "external/glm/glm/**.hpp",
    "external/glm/glm/**.inl",
}

defines
{
    "_CRT_SECURE_NO_WARNINGS",
    "GLFW_INCLUDE_NONE"
}

includedirs
{
    "src",
    "external/spdlog/include",
    "%{IncludeDir.GLFW}",
    "%{IncludeDir.Glad}",
    "%{IncludeDir.ImGui}",
    "%{IncludeDir.glm}",
    "%{IncludeDir.stb_image}",
    "%{IncludeDir.entt}",
    "%{IncludeDir.yaml_cpp}",
}

links
{
    "GLFW",
    "Glad",
    "ImGui",
    "opengl32.lib",
    "stb_image",
    "yaml-cpp",
}

-- filters are used to apply property to some specific configurations only
filter "system:windows"
    systemversion "latest" -- windows SDK version

    defines
    {
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