#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Settings/WindowSettings.h"

#include "AmberEngine/Context/Device.h"

namespace AmberEngine::Context
{
	class API_AMBERENGINE Window
	{
	public:
		Eventing::Event<int> KeyPressedEvent;
		Eventing::Event<int> KeyReleasedEvent;
		Eventing::Event<int> MouseButtonPressedEvent;
		Eventing::Event<int> MouseButtonReleasedEvent;
		
		Eventing::Event<uint16_t, uint16_t> ResizeEvent;
		Eventing::Event<uint16_t, uint16_t> FramebufferResizeEvent;
		
	public:
		static Window* FindInstance(GLFWwindow* p_glfwWindow);
		
		Window(Context::Device& p_device, const Settings::WindowSettings& p_windowSettings);
		~Window();

		void SetWindowUserPointer();
		void MakeCurrentContext() const;
		void SwapBuffers() const;
		void Restore() const;
		void Hide() const;
		void SetShouldClose(bool p_value) const;
		void SetCursorModeLock() const;
		void SetCursorModeFree() const;

		void SetViewport(int p_width, int p_height) const;

		void SetSize(uint16_t p_width, uint16_t p_height);

		bool ShouldClose() const;
		bool IsActive()		const;
		bool isFullscreen() const;
		
		GLFWwindow* GetGlfwWindow() const;

		std::pair<uint16_t, uint16_t> GetSize() const;

		int GetKey(const int p_key) const;

	private:
		void CreateGlfwWindow();

		void BindKeyCallback() const;
		void BindMouseCallback() const;
		void BindResizeCallback() const;
		void BindFramebufferResizeCallback() const;
		
		void OnResizeWindow(uint16_t p_width, uint16_t p_height);
		void OnResizeFramebuffer(uint16_t p_width, uint16_t p_height);

	private:
		static std::unordered_map<GLFWwindow*, Window*> __WINDOWS_MAP;

		GLFWwindow* m_glfwWindow;
		
		Device& m_device;

		std::string m_title;
		std::pair<uint16_t, uint16_t> m_size;
		bool m_fullscreen;
	};
}
