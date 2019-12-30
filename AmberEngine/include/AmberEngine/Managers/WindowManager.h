#pragma once

#include <memory>

#include "AmberEngine/Context/Device.h"
#include "AmberEngine/Context/Driver.h"
#include "AmberEngine/Context/Window.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Managers
{
	/**
	* Temporary class
	*/
	
	class API_AMBERENGINE WindowManager
	{
	private:
		Context::Device m_device;
		Context::Window m_window;
		Context::Driver m_driver;

	public:
		WindowManager(const Settings::DeviceSettings& p_deviceSettings, const Settings::WindowSettings& p_windowSettings, const Settings::DriverSettings& p_driverSettings);
		~WindowManager();

		bool IsOpen();
		
		Context::Device& GetDevice();
		Context::Window& GetWindow();
		Context::Driver& GetDriver();
	};
}