#pragma once

#include "AmberEngine/ImGUI/imgui.h"

namespace AmberEngine::UI
{
	class Canvas;
}

namespace AmberEngine::Core
{
	class UIManager
	{
	public:
		UIManager(GLFWwindow* p_glfwWindow, const std::string& p_glslVersion = "#version 150");
		~UIManager();

		bool LoadFont(const std::string& p_id, const std::string& p_path, float p_fontSize);
		bool UnloadFont(const std::string& p_id);
		bool UseFont(const std::string& p_id);
		void UseDefaultFont();
		void EnableDocking(bool p_value);
		void SetCanvas(UI::Canvas& p_canvas);
		void RemoveCanvas();
		void Render() const;

	private:
		void ApplyStyle();

	private:
		bool m_dockingState;
		UI::Canvas* m_canvas;
		std::unordered_map<std::string, ImFont*> m_fonts;
	};
}
