#include <iostream>

#include "AmberEngine/Context/Driver.h"
#include "AmberEngine/Debug/GLDebug.h"

AmberEngine::Core::Driver::Driver() : m_isActive(false)
{
}

void AmberEngine::Core::Driver::Setup()
{
	GLCall(const GLenum error = glewInit());
	if (error != GLEW_OK)
	{
		std::cout << "Error Init GLEW: " << glewGetErrorString(error) << std::endl;
	}

	m_isActive = true;

	GLCall(glEnable(GL_DEBUG_OUTPUT));
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_MULTISAMPLE);

	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(Debug::GLDebug::GLDebugMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	DisplayDriverInfo();
}

void AmberEngine::Core::Driver::Close()
{
	//TODO LOG DRIVER CLOSED
}

bool AmberEngine::Core::Driver::IsActive()
{
	return m_isActive;
}

void AmberEngine::Core::Driver::DisplayDriverInfo()
{
	std::cout << "Using GLEW: " << glewGetString(GLEW_VERSION) << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}