#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/ImGui/imgui.h"
#include "AmberEngine/ImGui/imgui_impl_glfw.h"
#include "AmberEngine/ImGui/imgui_impl_opengl3.h"

namespace AmberEngine::Manager
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
