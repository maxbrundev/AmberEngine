#include "Amberpch.h"

#include "AmberEngine/Context/Driver.h"

#include "AmberEngine/Debug/GLDebug.h"

AmberEngine::Context::Driver::Driver(const Settings::DriverSettings& p_driverSettings)
{
	InitGlew();

	isActive = true;

	if (p_driverSettings.enableDebugCallback)
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
	
	if (p_driverSettings.enableDepthTest)
		glEnable(GL_DEPTH_TEST);

	if (p_driverSettings.enableBackFaceCulling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if (p_driverSettings.enableBlend)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if(p_driverSettings.enableMultisample)
		glEnable(GL_MULTISAMPLE);

	DisplayDriverInfo();
}

void AmberEngine::Context::Driver::InitGlew()
{
	const GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::string message = "Error Init GLEW: ";
		std::string glewError = reinterpret_cast<const char*>(glewGetErrorString(error));

		std::cout << message + glewError << std::endl;
	}
}

void AmberEngine::Context::Driver::DisplayDriverInfo()
{
	std::cout << "Using GLEW: " << glewGetString(GLEW_VERSION) << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}