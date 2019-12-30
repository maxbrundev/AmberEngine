#pragma once

#include <GLFW/glfw3.h>

#include "AmberEngine/Settings/DeviceSettings.h"
#include "AmberEngine/Eventing/Event.h"
#include "AmberEngine/Context/EDeviceError.h"

namespace AmberEngine::Context
{
	class API_AMBERENGINE Device
	{
	private:
		bool m_isAlive = false;
		bool m_vsync = false;
		
	public:
		uint64_t m_errorDeviceListener;
		
	public:
		Device(const Settings::DeviceSettings& p_deviceSettings);
		~Device();

		void InitGLFW();
		void SetVsync(bool p_value);
		void PollEvents() const;
		
		bool HasVsync() const;

		float GetElapsedTime() const;

		//static AmberEngine::Eventing::Event<EDeviceError, std::string> ErrorEvent;
		
	private:
		void BindErrorCallback();
		void OnError(EDeviceError p_error, std::string p_message);
	};
}
