#pragma once

#include <GLFW/glfw3.h>

#include "Context/Icontext.h"

namespace  RenderEngine
{
	namespace Core
	{
		enum class State
		{
			INIT,
			CLOSE
		};

		class Context : public IContext
		{
		private:
			GLFWwindow* m_window{};

			static uint16_t m_width;
			static uint16_t m_height;

			State m_state;

			bool m_isActive;

		public:
			Context();
			~Context();

			void Setup() override;
			void Close() override;
			void Update() override;

			bool IsActive() override;

			void HandleInput();
			void HandleState();

			static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

			GLFWwindow* GetContextWindow() const;

			uint16_t GetWidthWindow() const;
			uint16_t GetHeightWindow() const;
		};
	}
}