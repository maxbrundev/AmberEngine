#include "AmberEngine/Context/Device.h"

#include <iostream>

uint16_t AmberEngine::Context::Device::m_width = 1280;
uint16_t AmberEngine::Context::Device::m_height = 720;

void AmberEngine::Context::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	std::cout << key << std::endl;
}

AmberEngine::Context::Device::Device(const Settings::DeviceSettings& p_settings)
{
	InitGLFW();

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, p_settings.enableDebugContext);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, p_settings.contextVersionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, p_settings.contextVersionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, p_settings.enableResizable);
	glfwWindowHint(GLFW_SAMPLES, 4);

	if(p_settings.enableDebugCallback)
	{
		glfwSetErrorCallback(error_callback);
	}

	InitWindow(p_settings.title);

	if(p_settings.enableFrameBufferSizeCallback)
	{
		glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	}
		
	glfwMakeContextCurrent(m_window);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetKeyCallback(m_window, key_callback);

	glfwSwapInterval(p_settings.enableVsync ? 1 : 0);
}

AmberEngine::Context::Device::~Device()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void AmberEngine::Context::Device::Close()
{
	glfwSetWindowShouldClose(m_window, true);
}

bool AmberEngine::Context::Device::IsActive()
{
	return !glfwWindowShouldClose(m_window);
}

void AmberEngine::Context::Device::InitGLFW()
{
	if (!glfwInit())
	{
		m_errors.push("Failed to Init GLFW");
	}
}

void AmberEngine::Context::Device::InitWindow(const char* p_title)
{
	m_window = glfwCreateWindow(m_width, m_height, p_title, nullptr, nullptr);
	if (!m_window)
	{
		m_errors.push("Failed to create GLFW window");
		glfwTerminate();
	}
}

void AmberEngine::Context::Device::LockCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void AmberEngine::Context::Device::FreeCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void AmberEngine::Context::Device::SwapBuffers()
{
	glfwSwapBuffers(m_window);
}

void AmberEngine::Context::Device::PollEvents()
{
	glfwPollEvents();
}

void AmberEngine::Context::Device::DisplayErrors()
{
	while (!m_errors.empty())
	{
		std::cout << m_errors.front().c_str() << std::endl;
		m_errors.pop();
	}
}

void AmberEngine::Context::Device::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	m_height = height;
	m_width = width;

	glViewport(0, 0, m_width, m_height);
}

void AmberEngine::Context::Device::error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

GLFWwindow* AmberEngine::Context::Device::GetContextWindow() const
{
	return m_window;
}

uint16_t AmberEngine::Context::Device::GetWindowWidth() const
{
	return m_width;
}

uint16_t AmberEngine::Context::Device::GetWindowHeight() const
{
	return m_height;
}

int AmberEngine::Context::Device::GetKey(const int p_key) const
{
	return glfwGetKey(m_window, p_key);
}

int AmberEngine::Context::Device::GetPressState()
{
	return GLFW_PRESS;
}

int AmberEngine::Context::Device::GetReleaseState()
{
	return GLFW_RELEASE;
}

const std::queue<std::string>& AmberEngine::Context::Device::GetQueueErros()
{
	return m_errors;
}
