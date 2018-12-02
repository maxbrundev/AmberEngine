#pragma once

#include <GLFW/glfw3.h>

#include "Context/Icontext.h"

namespace  RenderEngine
{
	namespace Core
	{
		class Device : public IContext
		{
		private:
			GLFWwindow* m_window{};

			static uint16_t m_width;
			static uint16_t m_height;

			////std::queue<std::string> m_errors;

		public:
			Device() = default;
			~Device();

			void Setup() override;
			void Close() override;
			bool IsActive() override;

			void InitGLFW();
			void InitWindow();

			void Update();
			void HandleInput();

			static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
			
			GLFWwindow* GetContextWindow() const;

			uint16_t GetWindowWidth() const;
			uint16_t GetWindowHeight() const;

			int GetKey(const int p_key) const;

			void LockCursor();
			void FreeCursor();

			static int GetPressState();
			static int GetReleaseState();

			//void DisplayErrors();
		};
	}
}