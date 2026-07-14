project "AmberEditor"
language "C++"
cppdialect "C++17"
pchheader "Amberpch.h"
pchsource "Amberpch.cpp"
files {"**.h", "**.inl", "**.cpp"}
includedirs {".", "include", "%{wks.location}/AmberDebug/include", "%{wks.location}/AmberMaths/include", "%{wks.location}/AmberTools/include", "%{wks.location}/AmberWindowing/include", "%{wks.location}/AmberRendering/include", "%{wks.location}/AmberPhysics/include", "%{wks.location}/AmberAudio/include", "%{wks.location}/AmberCore/include", "%{wks.location}/AmberUI/include", dependdir .. "ImGui/include", dependdir .. "tinyxml2/include", dependdir .. "glfw/include", dependdir .. "glm/include", dependdir .. "stb_image/include", dependdir .. "assimp/include", dependdir .. "bullet3/include", dependdir .. "glew/include", dependdir .. "irrklang/include", dependdir .. "lua/include"}
libdirs {dependdir .. "glfw/lib/%{cfg.platform}", dependdir .. "bullet3/lib/%{cfg.platform}/%{cfg.buildcfg}", dependdir .. "glew/lib/%{cfg.platform}", dependdir .. "assimp/lib/%{cfg.platform}", dependdir .. "irrklang/lib", dependdir .. "lua/lib"}
links {"AmberDebug", "AmberMaths", "AmberTools", "AmberWindowing", "AmberRendering", "AmberPhysics", "AmberAudio", "AmberCore", "AmberUI", "ImGui", "tinyxml2", "assimp-vc141-mt", "Bullet3Collision", "Bullet3Common", "Bullet3Dynamics", "Bullet3Geometry", "BulletCollision", "BulletDynamics", "BulletSoftBody", "LinearMath", "glew32", "glfw3dll", "opengl32", "irrKlang", "lua53"}
targetdir(outputdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")
objdir(objoutdir .. "%{cfg.platform}/%{cfg.buildcfg}/%{prj.name}")
characterset("MBCS")
debugdir "%{wks.location}/../../Build/%{prj.name}/%{cfg.platform}/%{cfg.buildcfg}"

postbuildcommands
{
	"xcopy /Y /I \"%{cfg.targetdir}\\*.exe\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\"",

	"xcopy /R /I /Y \"%{wks.location}..\\..\\Dependencies\\glfw\\bin\\%{cfg.platform}\\*.dll\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /R /I /Y \"%{wks.location}..\\..\\Dependencies\\glew\\bin\\%{cfg.platform}\\*.dll\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /R /I /Y \"%{wks.location}..\\..\\Dependencies\\assimp\\bin\\%{cfg.platform}\\*.dll\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /R /I /Y \"%{wks.location}..\\..\\Dependencies\\irrklang\\bin\\*.dll\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\"",
	"xcopy /R /I /Y \"%{wks.location}..\\..\\Dependencies\\lua\\bin\\*.dll\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\"",

	"xcopy /E /R /I /Y /Q \"%{wks.location}..\\..\\Resources\\Engine\\*\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\\Data\\Engine\"",
	"xcopy /E /R /I /Y /Q \"%{wks.location}..\\..\\Resources\\Editor\\*\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\\Data\\Editor\"",

	"xcopy /Y /I \"%{prj.location}Layout.ini\" \"%{wks.location}..\\..\\Build\\%{prj.name}\\%{cfg.platform}\\%{cfg.buildcfg}\\Config\\\"",

	"EXIT /B 0"
}

buildoptions {"/sdl", "/bigobj"}
defines {"_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING"}

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
