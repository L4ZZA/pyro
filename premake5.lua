include "./vendor/premake/premake_customization/solution_items.lua"

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
IncludeDir["GLFW"]      = "%{wks.location}/pyro/external/GLFW/include"
IncludeDir["Glad"]      = "%{wks.location}/pyro/external/Glad/include"
IncludeDir["glm"]       = "%{wks.location}/pyro/external/glm"
IncludeDir["stb_image"] = "%{wks.location}/pyro/external/stb_image"
IncludeDir["ImGui"]     = "%{wks.location}/pyro/external/imgui"
IncludeDir["entt"]      = "%{wks.location}/pyro/external/entt/include"
IncludeDir["assimp"]    = "%{wks.location}/pyro/external/assimp/include/"
IncludeDir["assimpcfg"] = "%{wks.location}/pyro/external/assimp/config/"

group "dependencies"
    include "vendor/premake"
    include "pyro/external/GLFW"
    include "pyro/external/Glad"
    include "pyro/external/imgui"
    include "pyro/external/stb_image"
    -- include "pyro/external/assimp"
group ""

-- engine core project
include "pyro"
-- ember application
include "ember"
-- sandbox application
include "sandbox"
-- Test application
include "tests"