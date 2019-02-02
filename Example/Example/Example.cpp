// Example.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#include "Application.h"

int main()
{
	AmberEngine::Settings::DeviceSettings deviceSettings{};

	deviceSettings.title = "AmberEngineV0.1.5";
	deviceSettings.contextVersionMajor = 4;
	deviceSettings.contextVersionMinor = 4;
	deviceSettings.enableResizable = true;
	deviceSettings.enableDebugContext = true;
	deviceSettings.enableVsync = true;
	deviceSettings.enableDebugCallback = true;
	deviceSettings.enableFrameBufferSizeCallback = true;

	AmberEngine::Settings::DriverSettings driverSettings{};

	driverSettings.enableDepthTest = true;
	driverSettings.enableBlend = true;
	driverSettings.enableBackFaceCulling = true;
	driverSettings.enableMultisample = true;
	driverSettings.enableDebugCallback = true;

	AmberEngine::Settings::RenderingSettings renderSettings{};

	renderSettings.deviceSettings = deviceSettings;
	renderSettings.driverSettings = driverSettings;
	
	Example::Application app(renderSettings);
	app.Setup();
	app.Run();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file