#pragma once

#include <GLFW/glfw3.h>

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Settings/DeviceSettings.h"
#include "AmberEngine/Context/EDeviceError.h"

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::Context
{
	class API_AMBERENGINE Device
	{
	private:
		bool m_isAlive = false;
		bool m_vsync = false;
		
	public:
		static Eventing::Event<EDeviceError, std::string> ErrorEvent;
		
		Device(const Settings::DeviceSettings& p_deviceSettings);
		~Device();

		void PollEvents() const;
		
		void SetVsync(bool p_value);
		bool HasVsync() const;
		
		float GetElapsedTime() const;
		
	private:
		void InitGLFW();
		void BindErrorCallback();
	};
}
