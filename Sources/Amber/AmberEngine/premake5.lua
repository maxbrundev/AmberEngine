project "AmberEngine"
kind "SharedLib"
language "C++"
cppdialect "C++17"
pchheader "Amberpch.h"
pchsource "Amberpch.cpp"
defines { "AMBERENGINE_EXPORT" }
files {"**.h", "**.inl", "**.cpp"}
includedirs {".", "include", dependdir .. "glfw/include", dependdir .. "glm/include", dependdir .. "stb_image/include", dependdir .. "assimp/include", dependdir .. "bullet3/include", dependdir .. "glew/include"}
libdirs {dependdir .. "glfw/lib/%{cfg.platform}", dependdir .. "bullet3/lib/%{cfg.platform}/%{cfg.buildcfg}", dependdir .. "glew/lib/%{cfg.platform}", dependdir .. "assimp/lib/%{cfg.platform}"}
links {"assimp-vc141-mt", "Bullet3Collision", "Bullet3Common", "Bullet3Dynamics", "Bullet3Geometry", "BulletCollision", "BulletDynamics", "BulletSoftBody", "LinearMath", "glew32", "glfw3dll", "opengl32"}

targetdir(outputdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")
objdir(objoutdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")
debugdir "%{wks.location}/../../Build/%{prj.name}/%{cfg.platform}/%{cfg.buildcfg}"

characterset("MBCS")

postbuildcommands 
{
	"xcopy /E /R /I /Y \"%{prj.location}include\\%{prj.name}\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\include\\%{prj.name}\"",
	"xcopy /E /R /I /Y \"%{prj.location}*.h\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\include\\%{prj.name}\"",

	"xcopy /Y /I \"%{cfg.targetdir}\\*.lib\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /Y /I \"%{cfg.targetdir}\\*.dll\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",

	"xcopy /E /R /I /Y \"%{wks.location}..\\..\\Dependencies\\assimp\\include\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\include\"",
	"xcopy /E /R /I /Y \"%{wks.location}..\\..\\Dependencies\\glew\\include\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\include\"",
	"xcopy /E /R /I /Y \"%{wks.location}..\\..\\Dependencies\\glfw\\include\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\include\"",
	"xcopy /E /R /I /Y \"%{wks.location}..\\..\\Dependencies\\glm\\include\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\include\"",
	"xcopy /E /R /I /Y \"%{wks.location}..\\..\\Dependencies\\stb_image\\include\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\include\"",
	"xcopy /E /R /I /Y \"%{wks.location}..\\..\\Dependencies\\bullet3\\include\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\include\"",

	"xcopy /R /I /Y \"%{wks.location}..\\..\\Dependencies\\glfw\\bin\\%{cfg.platform}\\*.dll\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /R /I /Y \"%{wks.location}..\\..\\Dependencies\\glew\\bin\\%{cfg.platform}\\*.dll\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /R /I /Y \"%{wks.location}..\\..\\Dependencies\\assimp\\bin\\%{cfg.platform}\\*.dll\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\bin\\%{cfg.platform}\\%{cfg.buildcfg}\"",

	"xcopy /R /I /Y \"%{wks.location}..\\..\\Dependencies\\glfw\\lib\\%{cfg.platform}\\*.lib\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /R /I /Y \"%{wks.location}..\\..\\Dependencies\\glew\\lib\\%{cfg.platform}\\*.lib\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /R /I /Y \"%{wks.location}..\\..\\Dependencies\\bullet3\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\\*.lib\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\lib\\%{cfg.platform}\\%{cfg.buildcfg}\"",

	"xcopy /D /I /Y /Q \"%{wks.location}..\\..\\Bin\\%{cfg.platform}\\Release\\%{prj.name}\\*.dll\" \"%{wks.location}..\\..\\Build\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /D /I /Y /Q \"%{wks.location}..\\..\\Bin\\%{cfg.platform}\\Release\\%{prj.name}\\*.exe\" \"%{wks.location}..\\..\\Build\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /D /I /Y /Q \"%{wks.location}..\\..\\Bin\\%{cfg.platform}\\Debug\\%{prj.name}\\*.dll\" \"%{wks.location}..\\..\\Build\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /D /I /Y /Q \"%{wks.location}..\\..\\Bin\\%{cfg.platform}\\Debug\\%{prj.name}\\*.exe\" \"%{wks.location}..\\..\\Build\\%{cfg.platform}\\%{cfg.buildcfg}\"",

	"EXIT /B 0"
}

buildoptions {"/sdl"}

filter "configurations:Debug"
defines {"DEBUG"}
symbols "On"
runtime "Debug"

filter "configurations:Release"
defines {"NDEBUG"}
optimize "On"
runtime "Release"
