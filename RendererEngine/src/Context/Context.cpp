#include "pch.h"

#include "Context/Context.h"

uint16_t RenderEngine::Core::Context::m_width = 1280;
uint16_t RenderEngine::Core::Context::m_height = 720;

RenderEngine::Core::Context::Context()
{
	m_isActive = false;
	Context::Setup();
}

void RenderEngine::Core::Context::Setup()
{
	if (!glfwInit())
	{
		std::cout << "Failed to Init GLFW" << std::endl;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = glfwCreateWindow(m_width, m_height, "Render Engine V0.1.4", nullptr, nullptr);
	if (!m_window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}

	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(m_window);

	m_isActive = true;
	m_state = State::INIT;
}

void RenderEngine::Core::Context::Close()
{
	m_state = State::CLOSE;
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool RenderEngine::Core::Context::IsActive()
{
	return m_isActive;
}

void RenderEngine::Core::Context::Update()
{
	HandleInput();
	HandleState();
}

void RenderEngine::Core::Context::HandleInput()
{
	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		m_state = State::CLOSE;

	if (glfwGetKey(m_window, GLFW_KEY_LEFT_ALT))
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

}

void RenderEngine::Core::Context::HandleState()
{
	if(m_state == State::CLOSE)
		glfwSetWindowShouldClose(m_window, true);
}

void RenderEngine::Core::Context::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	m_height = height;
	m_width = width;

	glViewport(0, 0, m_width, m_height);
}

GLFWwindow* RenderEngine::Core::Context::GetContextWindow() const
{
	return m_window;
}

uint16_t RenderEngine::Core::Context::GetWidthWindow() const
{
	return m_width;
}

uint16_t RenderEngine::Core::Context::GetHeightWindow() const
{
	return m_height;
}