#include "AmberEngine/Context/Window.h"
#include "AmberEngine/Managers/InputManager.h"
#include <iostream>

AmberEngine::Context::Window::Window(const Context::Device& p_device, const Settings::WindowSettings& p_windowSettings)
: m_device(p_device), m_title(p_windowSettings.title), m_size{ p_windowSettings.width, p_windowSettings.height }
{
	glfwWindowHint(GLFW_RESIZABLE, p_windowSettings.enableResizable);
	glfwWindowHint(GLFW_SAMPLES, p_windowSettings.samples);
	
	InitWindow();

	if (p_windowSettings.enableFrameBufferSizeCallback)
	{
		glfwSetFramebufferSizeCallback(m_glfwWindow, framebuffer_size_callback);
	}

	MakeCurrentContext();

	glfwSetKeyCallback(m_glfwWindow, Managers::InputManager::key_callback);
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_glfwWindow, Managers::InputManager::cursor_position_callback);

	//m_device.SetVsync(p_windowSettings.enableVsync);
}

AmberEngine::Context::Window::~Window()
{
	glfwDestroyWindow(m_glfwWindow);
}

void AmberEngine::Context::Window::Close()
{
	glfwSetWindowShouldClose(m_glfwWindow, true);
}

bool AmberEngine::Context::Window::IsActive()
{
	return !glfwWindowShouldClose(m_glfwWindow);
}

void AmberEngine::Context::Window::InitWindow()
{
	m_glfwWindow = glfwCreateWindow(static_cast<int>(m_size.first), static_cast<int>(m_size.second), m_title.c_str(), nullptr, nullptr);
	if (!m_glfwWindow)
	{
		m_errors.push("Failed to create GLFW window");
		glfwTerminate();
	}
}

void AmberEngine::Context::Window::LockCursor()
{
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void AmberEngine::Context::Window::FreeCursor()
{
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void AmberEngine::Context::Window::MakeCurrentContext() const
{
	glfwMakeContextCurrent(m_glfwWindow);
}

void AmberEngine::Context::Window::SwapBuffers() const
{
	glfwSwapBuffers(m_glfwWindow);
}

void AmberEngine::Context::Window::DisplayErrors()
{
	while (!m_errors.empty())
	{
		std::cout << m_errors.front().c_str() << std::endl;
		m_errors.pop();
	}
}

void AmberEngine::Context::Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//m_size.first = width;
	//m_size.second = height;

	//glViewport(0, 0, m_width, m_height);
}

GLFWwindow* AmberEngine::Context::Window::GetContextWindow() const
{
	return m_glfwWindow;
}

uint16_t AmberEngine::Context::Window::GetWindowWidth() const
{
	return m_size.first;
}

uint16_t AmberEngine::Context::Window::GetWindowHeight() const
{
	return m_size.second;
}

int AmberEngine::Context::Window::GetKey(const int p_key) const
{
	return glfwGetKey(m_glfwWindow, p_key);
}

int AmberEngine::Context::Window::GetPressState()
{
	return GLFW_PRESS;
}

int AmberEngine::Context::Window::GetReleaseState()
{
	return GLFW_RELEASE;
}
