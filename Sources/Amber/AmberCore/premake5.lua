project "AmberCore"
kind "StaticLib"
language "C++"
cppdialect "C++17"
pchheader "Amberpch.h"
pchsource "Amberpch.cpp"
files {"**.h", "**.inl", "**.cpp"}
includedirs {".", "include", "%{wks.location}/AmberDebug/include", "%{wks.location}/AmberMaths/include", "%{wks.location}/AmberTools/include", "%{wks.location}/AmberWindowing/include", "%{wks.location}/AmberRendering/include", "%{wks.location}/AmberPhysics/include", "%{wks.location}/AmberAudio/include", "%{wks.location}/AmberUI/include", dependdir .. "ImGui/include", dependdir .. "tinyxml2/include", dependdir .. "glm/include", dependdir .. "glfw/include", dependdir .. "glew/include", dependdir .. "bullet3/include", dependdir .. "irrklang/include", dependdir .. "assimp/include", dependdir .. "stb_image/include", dependdir .. "lua/include"}

targetdir(outputdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")
objdir(objoutdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")

characterset("MBCS")

buildoptions {"/sdl", "/bigobj"}
defines {"_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"}

filter "configurations:Debug"
defines {"DEBUG"}
symbols "On"
runtime "Debug"

filter "configurations:Release"
defines {"NDEBUG"}
optimize "On"
runtime "Release"
