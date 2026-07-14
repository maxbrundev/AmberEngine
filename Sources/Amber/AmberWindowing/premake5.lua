project "AmberWindowing"
kind "StaticLib"
language "C++"
cppdialect "C++17"
pchheader "Amberpch.h"
pchsource "Amberpch.cpp"
files {"**.h", "**.inl", "**.cpp"}
includedirs {".", "include", "%{wks.location}/AmberDebug/include", "%{wks.location}/AmberTools/include", dependdir .. "glfw/include"}

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
