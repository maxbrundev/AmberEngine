#include "pch.h"

#include <iostream>
#include <AmberEngine/Tools/Utils/Defines.h>
FORCE_DEDICATED_GPU

#include "Core/Application.h"

int main()
{
	AmberEngine::Settings::DeviceSettings deviceSettings;
	deviceSettings.contextVersionMajor = 4;
	deviceSettings.contextVersionMinor = 3;
	deviceSettings.debugProfile = true;

	AmberEngine::Settings::WindowSettings windowSettings;
	windowSettings.title = "AmberEngine v0.5.1";
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
	driverSettings.enableDebugCallback = true;

	const auto listenerId = AmberEngine::Context::Device::ErrorEvent += [](AmberEngine::Context::EDeviceError device_error, std::string error_message)
	{
		error_message = "AmberEngine requires OpenGL 4.3 or newer.\r\n" + error_message;
		std::cout << error_message.c_str() << std::endl;
	};

	const std::unique_ptr<Core::Application> exampleApp = std::make_unique<Core::Application>(deviceSettings, windowSettings, driverSettings);

	AmberEngine::Context::Device::ErrorEvent -= listenerId;

	exampleApp->Setup();
	exampleApp->Run();

	return 0;
}