#include <iostream>

#include "AmberEngine/Context/Driver.h"
#include "AmberEngine/Debug/GLDebug.h"

AmberEngine::Core::Driver::Driver(const DriverSettings& p_settings)
{
	GLCall(const GLenum error = glewInit());
	if (error != GLEW_OK)
	{
		std::string message = "Error Init GLEW: ";
		std::string glewError = reinterpret_cast<const char*>(glewGetErrorString(error));

		m_errors.push(message + glewError);
	}

	if (p_settings.enableDepthTest)
		glEnable(GL_DEPTH_TEST);

	if (p_settings.enableBlend)
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	}

	if(p_settings.enableBackFaceCulling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if(p_settings.enableMultisample)
		glEnable(GL_MULTISAMPLE);

	if(p_settings.enableDebugCallback)
	{
		GLint flags;
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(Debug::GLDebug::GLDebugMessageCallback, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	}

	DisplayDriverInfo();
}

void AmberEngine::Core::Driver::DisplayErrors()
{
	while (!m_errors.empty())
	{
		std::cout << m_errors.front().c_str() << std::endl;
		m_errors.pop();
	}
}

void AmberEngine::Core::Driver::DisplayDriverInfo()
{
	std::cout << "Using GLEW: " << glewGetString(GLEW_VERSION) << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

const std::queue<std::string>& AmberEngine::Core::Driver::GetQueueErros()
{
	return m_errors;
}