#pragma once

#include <iostream>
#include <queue>

#include <GLFW/glfw3.h>

#include "AmberEngine/Context/IContext.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Core
{
	struct API_AMBERENGINE DeviceSettings
	{
		const char* title;

		int contextVersionMajor;
		int contextVersionMinor;

		bool enableResizable;
		bool enableDebugContext;
		bool enableVsync;
		bool enableDebugCallback;
		bool enableFrameBufferSizeCallback;
	};

	class Device
	{
	private:
		GLFWwindow* m_window{};

		static uint16_t m_width;
		static uint16_t m_height;

		std::queue<std::string> m_errors;

	public:
		Device(const DeviceSettings& p_settings);
		~Device();

		void Close();
		bool IsActive();

		void InitGLFW();
		void InitWindow(const char* p_title);

		void LockCursor();
		void FreeCursor();

		void SwapBuffers();
		void PollEvents();

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
