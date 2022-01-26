#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE UIManager
	{
	public:
		UIManager(GLFWwindow* p_glfwWindow, const std::string& p_glslVersion = "#version 150");
		~UIManager();

		void EnableDocking(bool p_value);
		void PreDraw();
		void PostDraw();

	private:
		bool m_dockingState;
	};
}
