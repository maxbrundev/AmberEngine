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
		bool m_fullscreen;

		std::queue<std::string> m_errors;
		
		static std::unordered_map<GLFWwindow*, Window*> __WINDOWS_MAP;
		
	public:
		Eventing::Event<uint16_t, uint16_t> FramebufferResizeEvent;
		
	public:
		Window(Context::Device& p_device, const Settings::WindowSettings& p_windowSettings);
		~Window();

		void InitWindow();
		void MakeCurrentContext() const;
		void Close() const;
		void SwapBuffers() const;

		static Window* FindInstance(GLFWwindow* p_glfwWindow);
		
		void LockCursor() const;
		void FreeCursor() const;

		bool IsActive() const;
		bool isFullscreen() const;
		
		void DisplayErrors();
		
		GLFWwindow* GetGlfwWindow() const;

		std::pair<uint16_t, uint16_t> GetSize() const;

		int GetKey(const int p_key) const;

		const std::queue<std::string>& GetQueueErros();

		static int GetPressState();
		static int GetReleaseState();
		

	private:
		void BindFramebufferResizeCallback() const;
		
		void OnResize(uint16_t p_width, uint16_t p_height);
	};
}
