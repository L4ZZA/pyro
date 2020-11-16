project "stb_image"
    kind "StaticLib"
    language "C++"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/external/%{prj.name}")
    objdir ("%{wks.location}/inter/" .. outputdir .. "/external/%{prj.name}")

    files {
    "stb_image.h",
    "stb_image.cpp"
    }

    filter "system:windows"
    systemversion "latest"
    cppdialect "C++17"
    staticruntime "On"

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