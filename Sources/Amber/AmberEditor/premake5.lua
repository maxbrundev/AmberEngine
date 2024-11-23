project "AmberEditor"
language "C++"
cppdialect "C++17"
pchheader "pch.h"
pchsource "pch.cpp"
files {"**.h", "**.inl", "**.cpp"}
includedirs {".", "include", dependdir .. "glfw/include", dependdir .. "glm/include", dependdir .. "stb_image/include", dependdir .. "bullet3/include", dependdir .. "glew/include", "%{wks.location}/AmberEngine/include"}
libdirs {dependdir .. "glfw/lib/%{cfg.platform}", dependdir .. "bullet3/lib/%{cfg.platform}/%{cfg.buildcfg}", dependdir .. "glew/lib/%{cfg.platform}", dependdir .. "assimp/lib/%{cfg.platform}"}
links {"assimp-vc141-mt", "Bullet3Collision", "Bullet3Common", "Bullet3Dynamics", "Bullet3Geometry", "BulletCollision", "BulletDynamics", "BulletSoftBody", "LinearMath", "glew32", "glfw3dll", "opengl32", "AmberEngine"}
targetdir(outputdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")
objdir(objoutdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")
characterset("MBCS")
debugdir "%{wks.location}/../../Build/%{prj.name}/%{cfg.platform}/%{cfg.buildcfg}"

postbuildcommands 
{
	"xcopy /Y /I \"%{cfg.targetdir}\\*.dll\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /Y /I \"%{cfg.targetdir}\\*.exe\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	
	"xcopy /E /R /I /Y \"%{wks.location}..\\..\\Build\\AmberEngine\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\\*.dll\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\"",

	"xcopy /E /R /I /Y /Q \"%{wks.location}..\\..\\Resources\\Engine\\*\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\\Data\\Engine\"",
	"xcopy /E /R /I /Y /Q \"%{wks.location}..\\..\\Resources\\Editor\\*\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\\Data\\Editor\"",

	"xcopy /Y /I \"%{prj.location}Layout.ini\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\\Config\\\"", 

	"EXIT /B 0"
}

buildoptions {"/sdl"}

filter "configurations:Debug"
defines {"DEBUG"}
symbols "On"
kind "ConsoleApp"
runtime "Debug"

filter "configurations:Release"
defines {"NDEBUG"}
optimize "On"
kind "WindowedApp"
runtime "Release"
