#include "Amberpch.h"

#include "AmberEngine/Context/Window.h"

AmberEngine::Eventing::Event<bool> AmberEngine::Context::Window::CloseEvent;

std::unordered_map<GLFWwindow*, AmberEngine::Context::Window*> AmberEngine::Context::Window::__WINDOWS_MAP;

AmberEngine::Context::Window::Window(Context::Device& p_device, const Settings::WindowSettings& p_windowSettings) :
	m_device(p_device),
	m_glfwWindow(nullptr),
	m_title(p_windowSettings.title),
	m_size{ p_windowSettings.width, p_windowSettings.height },
	m_fullscreen(p_windowSettings.fullScreen)
{
	glfwWindowHint(GLFW_RESIZABLE, p_windowSettings.resizable);
	glfwWindowHint(GLFW_SAMPLES, p_windowSettings.samples);
	
	CreateGlfwWindow();

	BindKeyCallback();
	BindMouseCallback();
	BindCursorPosCallback();
	BindResizeCallback();
	BindFramebufferResizeCallback();
	
	MakeCurrentContext();

	SetCursorMode(AmberEngine::Context::ECursorMode::NORMAL);

	m_device.SetVsync(p_windowSettings.vsync);

	CloseEvent.AddListener(std::bind(&Window::SetShouldClose, this, std::placeholders::_1));
	ResizeEvent.AddListener(std::bind(&Window::OnResizeWindow, this, std::placeholders::_1, std::placeholders::_2));
	FramebufferResizeEvent.AddListener(std::bind(&Window::OnResizeFramebuffer, this, std::placeholders::_1, std::placeholders::_2));
}

AmberEngine::Context::Window::~Window()
{
	glfwDestroyWindow(m_glfwWindow);
}

void AmberEngine::Context::Window::CreateGlfwWindow()
{
	GLFWmonitor* primaryMonitor = nullptr;

	if (m_fullscreen)
		primaryMonitor = glfwGetPrimaryMonitor();
	
	m_glfwWindow = glfwCreateWindow(static_cast<int>(m_size.first), static_cast<int>(m_size.second), m_title.c_str(), primaryMonitor, nullptr);

	if (!m_glfwWindow)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW Window");
	}

	__WINDOWS_MAP[m_glfwWindow] = this;
}

void AmberEngine::Context::Window::SetWindowUserPointer() 
{
	glfwSetWindowUserPointer(m_glfwWindow, static_cast<void*>(this));
}

void AmberEngine::Context::Window::MakeCurrentContext() const
{
	glfwMakeContextCurrent(m_glfwWindow);
}

void AmberEngine::Context::Window::SetShouldClose(bool p_value) const
{
	glfwSetWindowShouldClose(m_glfwWindow, p_value);
}

void AmberEngine::Context::Window::SwapBuffers() const
{
	glfwSwapBuffers(m_glfwWindow);
}

AmberEngine::Context::Window* AmberEngine::Context::Window::FindInstance(GLFWwindow* p_glfwWindow)
{
	return __WINDOWS_MAP.find(p_glfwWindow) != __WINDOWS_MAP.end() ? __WINDOWS_MAP[p_glfwWindow] : nullptr;
}

void AmberEngine::Context::Window::SetCursorMode(ECursorMode p_cursorMode)
{
	m_cursorMode = p_cursorMode;
	glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, static_cast<int>(p_cursorMode));
}

void AmberEngine::Context::Window::SetViewport(int p_width, int p_height) const
{
	glViewport(0, 0, p_width, p_height);
}

void AmberEngine::Context::Window::SetSize(uint16_t p_width, uint16_t p_height)
{
	m_size.first = p_width;
	m_size.second = p_height;

	glfwSetWindowSize(m_glfwWindow, static_cast<int>(m_size.first), static_cast<int>(m_size.second));
}

void AmberEngine::Context::Window::Restore() const
{
	glfwRestoreWindow(m_glfwWindow);
}

void AmberEngine::Context::Window::SetIconFromMemory(uint8_t * p_data, uint32_t p_width, uint32_t p_height) const
{
	GLFWimage images[1];
	images[0].pixels = p_data;
	images[0].height = p_width;
	images[0].width = p_height;

	glfwSetWindowIcon(m_glfwWindow, 1, images);
}

void AmberEngine::Context::Window::Hide() const
{
	glfwHideWindow(m_glfwWindow);
}

bool AmberEngine::Context::Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_glfwWindow);
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

AmberEngine::Context::ECursorMode AmberEngine::Context::Window::GetCursorMode() const
{
	return m_cursorMode;
}

void AmberEngine::Context::Window::BindKeyCallback() const
{
	auto keyCallback = [](GLFWwindow* p_window, int p_key, int p_scancode, int p_action, int p_mods)
	{
		Window* windowInstance = FindInstance(p_window);

		if(windowInstance)
		{
			if (p_action == GLFW_PRESS)
			{
				windowInstance->KeyPressedEvent.Invoke(p_key);
			}

			if (p_action == GLFW_RELEASE)
			{
				windowInstance->KeyReleasedEvent.Invoke(p_key);
			}
		}
	};

	glfwSetKeyCallback(m_glfwWindow, keyCallback);
}

void AmberEngine::Context::Window::BindMouseCallback() const
{
	auto mouseCallback = [](GLFWwindow* p_window, int p_button, int p_action, int p_mods)
	{
		Window* windowInstance = FindInstance(p_window);

		if(windowInstance)
		{
			if (p_action == GLFW_PRESS)
			{
				windowInstance->MouseButtonPressedEvent.Invoke(p_button);
			}

			if (p_action == GLFW_RELEASE)
			{
				windowInstance->MouseButtonReleasedEvent.Invoke(p_button);
			}
		}
	};

	glfwSetMouseButtonCallback(m_glfwWindow, mouseCallback);
}

void AmberEngine::Context::Window::BindCursorPosCallback() const
{
	auto cursorPosCallback = [](GLFWwindow* p_window, double p_xpos, double p_ypos)
	{
		Window* windowInstance = FindInstance(p_window);

		if (windowInstance)
		{
			windowInstance->CursorPositionEvent.Invoke(p_xpos, p_ypos);
		}
	};

	glfwSetCursorPosCallback(m_glfwWindow, cursorPosCallback);
}

void AmberEngine::Context::Window::BindResizeCallback() const
{
	auto resizeCallback = [](GLFWwindow* p_window, int p_width, int p_height)
	{
		Window* windowInstance = FindInstance(p_window);

		if(windowInstance)
		{
			windowInstance->ResizeEvent.Invoke(static_cast<uint16_t>(p_width), static_cast<uint16_t>(p_height));
		}
	};
	
	glfwSetWindowSizeCallback(m_glfwWindow, resizeCallback);
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

void AmberEngine::Context::Window::OnResizeWindow(uint16_t p_width, uint16_t p_height)
{
	int width, height;
	glfwGetWindowSize(m_glfwWindow, &width, &height);
	
	m_size.first = width;
	m_size.second = height;
}

void AmberEngine::Context::Window::OnResizeFramebuffer(uint16_t p_width, uint16_t p_height)
{
	/*
	 * While the size of a window is measured in screen coordinates,
	 * OpenGL works with pixels. The size you pass into glViewport,
	 * for example, should be in pixels. On some machines screen coordinates and pixels are the same,
	 * but on others they will not be.
	 * To prevent wrong Window Size get the Window Size in screen coordinates with glfwGetWindowSize before assign the new Width and Height
	 */
	
	SetViewport(p_width, p_height);
	
	int width, height;
	glfwGetWindowSize(m_glfwWindow, &width, &height);

	m_size.first = width;
	m_size.second = height;
}
