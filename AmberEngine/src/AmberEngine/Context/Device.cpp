#include "Amberpch.h"

#include "AmberEngine/Context/Device.h"

AmberEngine::Eventing::Event<AmberEngine::Context::EDeviceError, std::string> AmberEngine::Context::Device::ErrorEvent;

AmberEngine::Context::Device::Device(const Settings::DeviceSettings& p_deviceSettings)
{
	InitGLFW();

	m_isAlive = true;

	if (p_deviceSettings.debugProfile)
		BindErrorCallback();
	
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, p_deviceSettings.debugProfile);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, p_deviceSettings.contextVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, p_deviceSettings.contextVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

AmberEngine::Context::Device::~Device()
{
	if(m_isAlive)
	{
		glfwTerminate();
	}
}

void AmberEngine::Context::Device::InitGLFW()
{
	int initializationCode = glfwInit();
	
	if (initializationCode == GLFW_FALSE)
	{
		throw std::runtime_error("Failed to Init GLFW");
		glfwTerminate();
	}
}

void AmberEngine::Context::Device::PollEvents() const
{
	glfwPollEvents();
}

void AmberEngine::Context::Device::SetVsync(bool p_value)
{
	glfwSwapInterval(p_value ? 1 : 0);
	m_vsync = p_value;
}

bool AmberEngine::Context::Device::HasVsync() const
{
	return m_vsync;
}

float AmberEngine::Context::Device::GetElapsedTime() const
{
	return static_cast<float>(glfwGetTime());
}

void AmberEngine::Context::Device::BindErrorCallback()
{
	auto errorCallback = [](int p_code, const char* p_description)
	{
		ErrorEvent.Invoke(static_cast<EDeviceError>(p_code), p_description);
	};
	
	glfwSetErrorCallback(errorCallback);
}
