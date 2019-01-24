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
		Core::Device m_device;
		Core::Driver m_driver;

	public:
		WindowManager(const Core::DeviceSettings& p_deviceSettings, const Core::DriverSettings& p_driverSettings);
		~WindowManager();

		bool IsOpen();

		Core::Device& GetDevice();
		Core::Driver& GetDriver();
	};
}