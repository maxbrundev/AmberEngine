#pragma once

#include <GLFW/glfw3.h>


#include "AmberWindowing/Settings/DeviceSettings.h"

#include "AmberWindowing/Context/EDeviceError.h"

#include "AmberTools/Eventing/Event.h"

namespace AmberWindowing::Context
{
	class Device
	{
	public:
		static AmberTools::Eventing::Event<EDeviceError, std::string> ErrorEvent;
		
		Device(const AmberWindowing::Settings::DeviceSettings& p_deviceSettings);
		~Device();

		void PollEvents() const;
		
		void SetVsync(bool p_value);
		bool HasVsync() const;
		
		float GetElapsedTime() const;
		std::pair<int16_t, int16_t> GetMonitorSize() const;

	private:
		void InitGLFW();
		void BindErrorCallback();

	private:
		bool m_isAlive = false;
		bool m_vsync = false;
	};
}
