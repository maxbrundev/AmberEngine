project "tinyxml2"
kind "StaticLib"
language "C++"
cppdialect "C++17"
files {"include/**.h", "src/**.cpp"}
includedirs {"include"}

targetdir(outputdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")
objdir(objoutdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")

characterset("MBCS")

filter "configurations:Debug"
defines {"DEBUG"}
symbols "On"
runtime "Debug"

filter "configurations:Release"
defines {"NDEBUG"}
optimize "On"
runtime "Release"
