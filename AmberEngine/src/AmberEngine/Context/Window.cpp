#include "AmberEngine/Context/Window.h"

#include <iostream>

#include "AmberEngine/Managers/InputManager.h"

std::unordered_map<GLFWwindow*, AmberEngine::Context::Window*>  AmberEngine::Context::Window::__WINDOWS_MAP;

AmberEngine::Context::Window::Window(Context::Device& p_device, const Settings::WindowSettings& p_windowSettings) :
	m_device(p_device),
	m_glfwWindow(nullptr),
	m_title(p_windowSettings.title),
	m_size{ p_windowSettings.width, p_windowSettings.height },
	m_fullscreen(p_windowSettings.fullScreen)
{
	glfwWindowHint(GLFW_RESIZABLE, p_windowSettings.resizable);
	glfwWindowHint(GLFW_SAMPLES, p_windowSettings.samples);
	
	InitWindow();
	
	BindFramebufferResizeCallback();
	
	MakeCurrentContext();

	glfwSetKeyCallback(m_glfwWindow, Managers::InputManager::key_callback);
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(m_glfwWindow, Managers::InputManager::cursor_position_callback);

	m_device.SetVsync(p_windowSettings.vsync);

	FramebufferResizeEvent.AddListener(std::bind(&Window::OnResize, this, std::placeholders::_1, std::placeholders::_2));
}

AmberEngine::Context::Window::~Window()
{
	glfwDestroyWindow(m_glfwWindow);
}

void AmberEngine::Context::Window::InitWindow()
{
	GLFWmonitor* primaryMonitor = nullptr;

	if (m_fullscreen)
		primaryMonitor = glfwGetPrimaryMonitor();
	
	m_glfwWindow = glfwCreateWindow(static_cast<int>(m_size.first), static_cast<int>(m_size.second), m_title.c_str(), primaryMonitor, nullptr);
	if (!m_glfwWindow)
	{
		m_errors.push("Failed to create GLFW window");
		glfwTerminate();
	}

	__WINDOWS_MAP[m_glfwWindow] = this;
}

void AmberEngine::Context::Window::MakeCurrentContext() const
{
	glfwMakeContextCurrent(m_glfwWindow);
}

void AmberEngine::Context::Window::Close() const
{
	glfwSetWindowShouldClose(m_glfwWindow, true);
}

void AmberEngine::Context::Window::SwapBuffers() const
{
	glfwSwapBuffers(m_glfwWindow);
}

AmberEngine::Context::Window* AmberEngine::Context::Window::FindInstance(GLFWwindow* p_glfwWindow)
{
	return __WINDOWS_MAP.find(p_glfwWindow) != __WINDOWS_MAP.end() ? __WINDOWS_MAP[p_glfwWindow] : nullptr;
}

void AmberEngine::Context::Window::LockCursor() const
{
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void AmberEngine::Context::Window::FreeCursor() const
{
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

bool AmberEngine::Context::Window::IsActive() const
{
	return !glfwWindowShouldClose(m_glfwWindow);
}

bool AmberEngine::Context::Window::isFullscreen() const
{
	return m_fullscreen;
}

GLFWwindow* AmberEngine::Context::Window::GetGlfwWindow() const
{
	return m_glfwWindow;
}

std::pair<uint16_t, uint16_t> AmberEngine::Context::Window::GetSize() const
{
	return m_size;
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

void AmberEngine::Context::Window::BindFramebufferResizeCallback() const
{
	auto framebufferResizeCallback = [](GLFWwindow* p_window, int p_width, int p_height)
	{
		Window* windowInstance = FindInstance(p_window);

		if (windowInstance)
		{
			windowInstance->FramebufferResizeEvent.Invoke(static_cast<uint16_t>(p_width), static_cast<uint16_t>(p_height));
		}
	};
	
	glfwSetFramebufferSizeCallback(m_glfwWindow, framebufferResizeCallback);
}

void AmberEngine::Context::Window::OnResize(uint16_t p_width, uint16_t p_height)
{
	m_size.first = p_width;
	m_size.second = p_height;
	
	glViewport(0, 0, m_size.first, m_size.second);
}

void AmberEngine::Context::Window::DisplayErrors()
{
	while (!m_errors.empty())
	{
		std::cout << m_errors.front().c_str() << std::endl;
		m_errors.pop();
	}
}