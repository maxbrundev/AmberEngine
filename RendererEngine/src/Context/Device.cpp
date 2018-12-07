#include "pch.h"

#include "Context/Device.h"

uint16_t RenderEngine::Core::Device::m_width = 1280;
uint16_t RenderEngine::Core::Device::m_height = 720;

RenderEngine::Core::Device::~Device()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void RenderEngine::Core::Device::Setup()
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

void RenderEngine::Core::Device::Close()
{
	glfwSetWindowShouldClose(m_window, true);
}

bool RenderEngine::Core::Device::IsActive()
{
	return !glfwWindowShouldClose(m_window);
}

void RenderEngine::Core::Device::InitGLFW()
{
	if (!glfwInit())
	{
		throw std::runtime_error("Failed to Init GLFW");
	}
}

void RenderEngine::Core::Device::InitWindow()
{
	m_window = glfwCreateWindow(m_width, m_height, "AmberEngine V0.1.5", nullptr, nullptr);
	if (!m_window)
	{
		throw std::runtime_error("Failed to create GLFW window");
	}
}

void RenderEngine::Core::Device::Update()
{
	HandleInput();
}

void RenderEngine::Core::Device::HandleInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		Close();

	/*if (glfwGetKey(m_window, GLFW_KEY_LEFT_ALT))
		FreeCursor();
	else
		LockCursor();*/
}

void RenderEngine::Core::Device::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	m_height = height;
	m_width = width;

	glViewport(0, 0, m_width, m_height);
}

GLFWwindow* RenderEngine::Core::Device::GetContextWindow() const
{
	return m_window;
}

uint16_t RenderEngine::Core::Device::GetWindowWidth() const
{
	return m_width;
}

uint16_t RenderEngine::Core::Device::GetWindowHeight() const
{
	return m_height;
}

int RenderEngine::Core::Device::GetKey(const int p_key) const
{
	return glfwGetKey(m_window, p_key);
}

void RenderEngine::Core::Device::LockCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void RenderEngine::Core::Device::FreeCursor()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void RenderEngine::Core::Device::SwapBuffers()
{
	glfwSwapBuffers(m_window);
	
}

void RenderEngine::Core::Device::PollEvents()
{
	glfwPollEvents();
}

int RenderEngine::Core::Device::GetPressState()
{
	return GLFW_PRESS;
}

int RenderEngine::Core::Device::GetReleaseState()
{
	return GLFW_RELEASE;
}

/*void RenderEngine::Core::Context::DisplayErrors()
{
	while (!m_errors.empty())
	{
		std::cout << "Errors: " << m_errors.front().c_str() << std::endl;
	}
}*/