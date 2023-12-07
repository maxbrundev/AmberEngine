#pragma once

#include "AmberEngine/ImGUI/imgui.h"

namespace AmberEngine::UI
{
	class Canvas;
}

namespace AmberEngine::UI::Core
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
		void SetCanvas(Canvas& p_canvas);
		void RemoveCanvas();
		void Render() const;
		void PostRender() const;

	private:
		void ApplyStyle();

	private:
		bool m_dockingState;
		Canvas* m_canvas;
		std::unordered_map<std::string, ImFont*> m_fonts;
	};
}
