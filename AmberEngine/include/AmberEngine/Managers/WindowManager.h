#pragma once

#include <memory>

#include "AmberEngine/Context/Device.h"
#include "AmberEngine/Context/Driver.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Managers
{
	class API_AMBERENGINE WindowManager
	{
	private:
		Context::Device m_device;
		Context::Driver m_driver;

	public:
		WindowManager(const Settings::DeviceSettings& p_deviceSettings, const Settings::DriverSettings& p_driverSettings);
		~WindowManager();

		bool IsOpen();

		Context::Device& GetDevice();
		Context::Driver& GetDriver();
	};
}