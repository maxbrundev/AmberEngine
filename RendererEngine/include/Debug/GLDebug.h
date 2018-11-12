#pragma once

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) RenderEngine::Debug::GLDebug::GLClearError();\
	x;\
	ASSERT(RenderEngine::Debug::GLDebug::GLLogCall(#x, __FILE__, __LINE__))

namespace RenderEngine
{
	namespace Debug
	{
		class GLDebug
		{
		public:
			static void GLClearError();
			static bool GLLogCall(const char* function, const char* file, int line);
			static void GLAPIENTRY GLDebugMessageCallback(GLenum source, GLenum type,
				GLuint id, GLenum severity, GLsizei length,
				const GLchar *message, const void *userParam);
		};
	}
}