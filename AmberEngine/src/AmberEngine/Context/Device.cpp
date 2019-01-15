#include <iostream>

#include "AmberEngine/Context/Device.h"


uint16_t AmberEngine::Core::Device::m_width = 1280;
uint16_t AmberEngine::Core::Device::m_height = 720;

AmberEngine::Core::Device::~Device()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void AmberEngine::Core::Device::Setup()
{
	try
	{
		/*if(!glfwInit())
		{
			throw std::runtime_error("Failed to Init GLFW");
		}*/
		InitGLFW();
	}
	catch (std::exception const& err)
	{
		std::cerr << err.what() << std::endl;
		return;
	}

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	try
	{
		/*m_window = glfwCreateWindow(m_width, m_height, "Render Engine V0.1.4", nullptr, nullptr);
		if (!m_window)
		{
			throw std::runtime_error("Failed to create GLFW window");
		}*/

		InitWindow();

	}
	catch (std::exception const& err)
	{
		std::cerr << err.what() << std::endl;
		glfwTerminate();
		return;
	}

	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
	glfwMakeContextCurrent(m_window);
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSwapInterval(0);

	//if (!m_errors.empty())
		//m_state = State::INIT;
	//else
	//	DisplayErrors();
}

void AmberEngine::Core::Device::Close()
{
	glfwSetWindowShouldClose(m_window, true);
}

bool AmberEngine::Core::Device::IsActive()
{
	return !glfwWindowShouldClose(m_window);
}

void AmberEngine::Core::Device::InitGLFW()
{
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to Init GLFW");
	}
}

void AmberEngine::Core::Device::InitWindow()
{
	m_window = glfwCreateWindow(m_width, m_height, "AmberEngine V0.1.5", nullptr, nullptr);
	if (!m_window)
	{
		throw std::runtime_error("Failed to create GLFW window");
	}
}

void AmberEngine::Core::Device::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	m_height = height;
	m_width = width;

	glViewport(0, 0, m_width, m_height);
}

GLFWwindow* AmberEngine::Core::Device::GetContextWindow() const
{
	return m_window;
}

uint16_t AmberEngine::Core::Device::GetWindowWidth() const
{
	return m_width;
}

uint16_t AmberEngine::Core::Device::GetWindowHeight() const
{
	return m_height;
}

int AmberEngine::Core::Device::GetKey(const int p_key) const
{
	return glfwGetKey(m_window, p_key);
}

void AmberEngine::Core::Device::LockCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void AmberEngine::Core::Device::FreeCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void AmberEngine::Core::Device::SwapBuffers()
{
	glfwSwapBuffers(m_window);
	
}

void AmberEngine::Core::Device::PollEvents()
{
	glfwPollEvents();
}

int AmberEngine::Core::Device::GetPressState()
{
	return GLFW_PRESS;
}

int AmberEngine::Core::Device::GetReleaseState()
{
	return GLFW_RELEASE;
}

/*void AmberEngine::Core::Context::DisplayErrors()
{
	while (!m_errors.empty())
	{
		std::cout << "Errors: " << m_errors.front().c_str() << std::endl;
	}
}*/