#pragma once


#include "AmberWindowing/Settings/WindowSettings.h"

#include "AmberWindowing/Context/Device.h"
#include "AmberWindowing/Context/ECursorMode.h"

namespace AmberWindowing::Context
{
	class Window
	{
	public:
		static AmberTools::Eventing::Event<bool> CloseEvent;

		AmberTools::Eventing::Event<int> KeyPressedEvent;
		AmberTools::Eventing::Event<int> KeyReleasedEvent;
		AmberTools::Eventing::Event<int> MouseButtonPressedEvent;
		AmberTools::Eventing::Event<int> MouseButtonReleasedEvent;
		AmberTools::Eventing::Event<double, double> CursorPositionEvent;

		AmberTools::Eventing::Event<uint16_t, uint16_t> ResizeEvent;
		AmberTools::Eventing::Event<uint16_t, uint16_t> FrameBufferResizeEvent;
		
	public:
		static Window* FindInstance(GLFWwindow* p_glfwWindow);
		
		Window(Device& p_device, const AmberWindowing::Settings::WindowSettings& p_windowSettings);
		~Window();

		void SetWindowUserPointer();
		void MakeCurrentContext() const;
		void SwapBuffers() const;
		void Restore() const;

		void SetIconFromMemory(uint8_t* p_data, uint32_t p_width, uint32_t p_height) const;

		void Hide() const;
		void SetShouldClose(bool p_value) const;
		void SetCursorMode(ECursorMode p_cursorMode);
		ECursorMode GetCursorMode() const;

		void SetSize(uint16_t p_width, uint16_t p_height);
		void SetPosition(int16_t p_x, int16_t p_y);
		bool ShouldClose() const;
		bool IsActive() const;
		bool isFullscreen() const;
		
		GLFWwindow* GetGlfwWindow() const;

		std::pair<uint16_t, uint16_t> GetSize() const;

		int GetKey(const int p_key) const;

	private:
		void CreateGlfwWindow();

		void BindKeyCallback() const;
		void BindMouseCallback() const;
		void BindCursorPosCallback() const;
		void BindResizeCallback() const;
		void BindFramebufferResizeCallback() const;
		
		void OnResizeWindow(uint16_t p_width, uint16_t p_height);
		void OnResizeFramebuffer(uint16_t p_width, uint16_t p_height);

	private:
		static std::unordered_map<GLFWwindow*, Window*> WINDOWS_MAP;

		GLFWwindow* m_glfwWindow;
		
		Device& m_device;

		std::string m_title;
		std::pair<uint16_t, uint16_t> m_size;
		bool m_fullscreen;
		ECursorMode m_cursorMode;
	};
}
