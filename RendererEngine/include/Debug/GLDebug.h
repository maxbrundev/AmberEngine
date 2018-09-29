#pragma once

//#ifdef _DEBUG
	#define ASSERT(x) if (!(x)) __debugbreak();
	#define GLCall(x) RenderEngine::Debug::GLDebug::GLClearError();\
		x;\
		ASSERT(RenderEngine::Debug::GLDebug::GLLogCall(#x, __FILE__, __LINE__))
/*#else
	#define GLCall(x) x; RenderEngine::Debug::GLDebug::GLLogCall(#x, __FILE__, __LINE__)
#endif*/

namespace RenderEngine
{
	namespace Debug
	{
		class GLDebug
		{
		public:
			static void GLClearError();
			static bool GLLogCall(const char* function, const char* file, int line);
		};
	}
}