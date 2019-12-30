#include "AmberEngine/Context/Device.h"

#include <iostream>

AmberEngine::Context::Device::Device(const Settings::DeviceSettings& p_deviceSettings)
{
	InitGLFW();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, p_deviceSettings.debugProfile);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, p_deviceSettings.contextVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, p_deviceSettings.contextVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//if (p_deviceSettings.debugProfile)
		//BindErrorCallback();
	
	m_isAlive = true;

	//m_errorDeviceListener = ErrorEvent.AddListener(std::bind(&Device::OnError, this, std::placeholders::_1, std::placeholders::_2));
}

AmberEngine::Context::Device::~Device()
{
	if(m_isAlive)
		glfwTerminate();

	//ErrorEvent.RemoveListener(m_errorDeviceListener);
}

void AmberEngine::Context::Device::InitGLFW()
{
	if (!glfwInit())
	{
		glfwTerminate();
		throw std::runtime_error("Failed to Init GLFW");
	}
}

void AmberEngine::Context::Device::SetVsync(bool p_value)
{
	glfwSwapInterval(p_value ? 1 : 0);
	m_vsync = p_value;
}

void AmberEngine::Context::Device::PollEvents() const
{
	glfwPollEvents();
}

bool AmberEngine::Context::Device::HasVsync() const
{
	return m_vsync;
}

float AmberEngine::Context::Device::GetElapsedTime() const
{
	return static_cast<float>(glfwGetTime());
}

void AmberEngine::Context::Device::OnError(EDeviceError p_error, std::string p_message)
{
	std::cout << p_message.c_str() << std::endl;
}

void AmberEngine::Context::Device::BindErrorCallback()
{
	auto errorCallback = [](int p_code, const char* p_description)
	{
		//ErrorEvent.Invoke(static_cast<EDeviceError>(p_code), p_description);
	};

	//glfwSetErrorCallback(errorCallback);
}
