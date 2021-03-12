#include "pch.h"

#include <AmberEngine/Core/Application.h>
#include <AmberEngine/Utils/Defines.h>
FORCE_DEDICATED_GPU

#include "ExampleLayer.h"

class Example : public AmberEngine::Core::Application
{
public:
	inline Example(const AmberEngine::Settings::DeviceSettings& p_deviceSettings, const AmberEngine::Settings::WindowSettings& p_windowSettings, const AmberEngine::Settings::DriverSettings& p_driverSettings) :
	Application(p_deviceSettings, p_windowSettings, p_driverSettings)
	{
		PushLayer(new ExampleLayer(m_context, m_editor));
	}
};

int main()
{
	AmberEngine::Settings::DeviceSettings deviceSettings;
	deviceSettings.contextVersionMajor = 4;
	deviceSettings.contextVersionMinor = 3;
	deviceSettings.debugProfile = true;
	
	AmberEngine::Settings::WindowSettings windowSettings;
	windowSettings.title = "AmberEngineV0.1.5";
	windowSettings.width = 1280;
	windowSettings.height = 720;
	windowSettings.resizable = true;
	windowSettings.vsync = true;
	windowSettings.samples = 4;

	AmberEngine::Settings::DriverSettings driverSettings;
	driverSettings.enableDepthTest = true;
	driverSettings.enableBlend = true;
	driverSettings.enableBackFaceCulling = true;
	driverSettings.enableMultisample = true;
	driverSettings.enableDebugCallback = true;

	std::unique_ptr<Example> exampleApp = std::make_unique<Example>(deviceSettings, windowSettings, driverSettings);
	
	exampleApp->Run();
}