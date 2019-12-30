#pragma once

#include <queue>

#include <GLFW/glfw3.h>

#include "AmberEngine/Settings/WindowSettings.h"
#include "AmberEngine/Context/Device.h"

namespace AmberEngine::Context
{
	class API_AMBERENGINE Window
	{
	private:
		Context::Device& m_device;
		GLFWwindow* m_glfwWindow;

		std::string m_title;
		std::pair<uint16_t, uint16_t> m_size;
		
		std::queue<std::string> m_errors;
		
	public:
		Window(Context::Device& p_device, const Settings::WindowSettings& p_windowSettings);
		~Window();

		void Close();
		bool IsActive();

		void InitWindow();

		void LockCursor();
		void FreeCursor();

		void MakeCurrentContext() const;
		void SwapBuffers() const;

		void DisplayErrors();

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

		GLFWwindow* GetContextWindow() const;

		uint16_t GetWindowWidth() const;
		uint16_t GetWindowHeight() const;

		int GetKey(const int p_key) const;

		static int GetPressState();
		static int GetReleaseState();

		const std::queue<std::string>& GetQueueErros();
	};
}
