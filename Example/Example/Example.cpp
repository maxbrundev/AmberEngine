#include "pch.h"



#include <AmberEngine/Tools/Utils/Defines.h>
FORCE_DEDICATED_GPU

#include "Application.h"

int main()
{
	AmberEngine::Settings::DeviceSettings deviceSettings;
	deviceSettings.contextVersionMajor = 4;
	deviceSettings.contextVersionMinor = 3;
	deviceSettings.debugProfile = false;
	
	AmberEngine::Settings::WindowSettings windowSettings;
	windowSettings.title = "AmberEngineV0.1.5";
	windowSettings.width = 1280;
	windowSettings.height = 720;
	windowSettings.resizable = true;
	windowSettings.vsync = false;
	windowSettings.samples = 4;

	AmberEngine::Settings::DriverSettings driverSettings;
	driverSettings.enableDepthTest = true;
	driverSettings.enableBlend = true;
	driverSettings.enableBackFaceCulling = true;
	driverSettings.enableMultisample = true;
	driverSettings.enableDebugCallback = false;

	std::unique_ptr<Example::Application> exampleApp = std::make_unique<Example::Application>(deviceSettings, windowSettings, driverSettings);
	exampleApp->Setup();
	exampleApp->Run();
}