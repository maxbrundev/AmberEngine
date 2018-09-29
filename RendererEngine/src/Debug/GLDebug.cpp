#include "pch.h"

#include "Debug/GLDebug.h"

void RenderEngine::Debug::GLDebug::GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool RenderEngine::Debug::GLDebug::GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}
