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
		std::unique_ptr<Core::Device> m_device;
		std::unique_ptr<Core::Driver> m_driver;

	public:
		WindowManager();
		~WindowManager();

		void Setup();

		bool IsOpen() const;

		Core::Device& GetDevice() const;
		Core::Driver& GetDriver() const;
	};
}