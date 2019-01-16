#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) AmberEngine::Debug::GLDebug::GLClearError();\
	x;\
	ASSERT(AmberEngine::Debug::GLDebug::GLLogCall(#x, __FILE__, __LINE__))

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Debug
{
	class API_AMBERENGINE GLDebug
	{
	public:
		static void GLClearError();
		static bool GLLogCall(const char* function, const char* file, int line);
		static void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type,
			GLuint id, GLenum severity, GLsizei length,
			const GLchar *message, const void *userParam);
	};
}