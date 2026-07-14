project "AmberRendering"
kind "StaticLib"
language "C++"
cppdialect "C++17"
pchheader "Amberpch.h"
pchsource "Amberpch.cpp"
files {"**.h", "**.inl", "**.cpp"}
includedirs {".", "include", "%{wks.location}/AmberDebug/include", "%{wks.location}/AmberMaths/include", "%{wks.location}/AmberTools/include", dependdir .. "glew/include", dependdir .. "glm/include", dependdir .. "stb_image/include", dependdir .. "assimp/include"}

targetdir(outputdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")
objdir(objoutdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")

characterset("MBCS")

buildoptions {"/sdl"}

filter "configurations:Debug"
defines {"DEBUG"}
symbols "On"
runtime "Debug"

filter "configurations:Release"
defines {"NDEBUG"}
optimize "On"
runtime "Release"
