#include "Amberpch.h"

#include "AmberWindowing/Context/Device.h"

AmberTools::Eventing::Event<AmberWindowing::Context::EDeviceError, std::string> AmberWindowing::Context::Device::ErrorEvent;

AmberWindowing::Context::Device::Device(const AmberWindowing::Settings::DeviceSettings& p_deviceSettings)
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

AmberWindowing::Context::Device::~Device()
{
	if(m_isAlive)
	{
		glfwTerminate();
	}
}

void AmberWindowing::Context::Device::InitGLFW()
{
	const int initializationCode = glfwInit();
	
	if (initializationCode == GLFW_FALSE)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to Init GLFW");
	}
}

void AmberWindowing::Context::Device::PollEvents() const
{
	glfwPollEvents();
}

void AmberWindowing::Context::Device::SetVsync(bool p_value)
{
	glfwSwapInterval(p_value ? 1 : 0);
	m_vsync = p_value;
}

bool AmberWindowing::Context::Device::HasVsync() const
{
	return m_vsync;
}

float AmberWindowing::Context::Device::GetElapsedTime() const
{
	return static_cast<float>(glfwGetTime());
}

std::pair<int16_t, int16_t> AmberWindowing::Context::Device::GetMonitorSize() const
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	return std::pair<int16_t, int16_t>(static_cast<int16_t>(mode->width), static_cast<int16_t>(mode->height));
}

void AmberWindowing::Context::Device::BindErrorCallback()
{
	auto errorCallback = [](int p_code, const char* p_description)
	{
		ErrorEvent.Invoke(static_cast<EDeviceError>(p_code), p_description);
	};
	
	glfwSetErrorCallback(errorCallback);
}
