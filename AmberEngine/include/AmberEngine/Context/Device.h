#pragma once

#include <queue>

#include <GLFW/glfw3.h>

#include "AmberEngine/Settings/DeviceSettings.h"

namespace AmberEngine::Context
{
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	class API_AMBERENGINE Device
	{
	private:
		GLFWwindow* m_window{};

		static uint16_t m_width;
		static uint16_t m_height;

		std::queue<std::string> m_errors;

	public:
		Device(const Settings::DeviceSettings& p_settings);
		~Device();

		void Close();
		bool IsActive();

		void InitGLFW();
		void InitWindow(const char* p_title);

		void LockCursor();
		void FreeCursor();

		void SwapBuffers();
		void PollEvents();
		//void SetCallBack();
		void DisplayErrors();

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void error_callback(int error, const char* description);
		
		GLFWwindow* GetContextWindow() const;

		uint16_t GetWindowWidth() const;
		uint16_t GetWindowHeight() const;

		int GetKey(const int p_key) const;
		static int GetPressState();
		static int GetReleaseState();

		const std::queue<std::string>& GetQueueErros();
	};
}
