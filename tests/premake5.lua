
project "tests"

kind "ConsoleApp"
language "C++"
cppdialect "C++17"
staticruntime "on"

targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
objdir ("%{wks.location}/inter/" .. outputdir .. "/%{prj.name}")

files
{
    "src/**.h",
    "src/**.cpp"
}

includedirs
{
    "src",
    "%{wks.location}/ember/src",
    "%{wks.location}/pyro/external/spdlog/include",
    "%{wks.location}/pyro/src",
    "%{wks.location}/pyro/external",
    "%{IncludeDir.glm}",
    "%{IncludeDir.ImGui}",
}

links
{
    "ember",
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
