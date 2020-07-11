#pragma once

#include <AmberEngine/Managers/RenderingManager.h>

namespace Example
{
	class Application
	{
	private:
		AmberEngine::Managers::RenderingManager m_renderer;
	public:
		Application(const AmberEngine::Settings::DeviceSettings& p_deviceSettings, const AmberEngine::Settings::WindowSettings& p_windowSettings, const AmberEngine::Settings::DriverSettings& p_driverSettings);
		~Application() = default;

		void Setup();
		void Run();
	};
}