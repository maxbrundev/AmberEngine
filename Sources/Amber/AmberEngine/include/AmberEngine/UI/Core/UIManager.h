#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/ImGUI/imgui.h"

namespace AmberEngine::UI
{
	class Canvas;
}

namespace AmberEngine::UI::Core
{
	class API_AMBERENGINE UIManager
	{
	public:
		UIManager(GLFWwindow* p_glfwWindow, const std::string& p_glslVersion = "#version 150");
		~UIManager();

		UIManager(const UIManager& other) = delete;
		UIManager(UIManager&& other) = delete;
		UIManager& operator=(const UIManager& other) = delete;
		UIManager& operator=(UIManager&& other) = delete;

		bool LoadFont(const std::string& p_id, const std::string& p_path, float p_fontSize);
		bool UnloadFont(const std::string& p_id);
		bool UseFont(const std::string& p_id);
		void UseDefaultFont();
		void EnableDocking(bool p_value);
		void SetCanvas(Canvas& p_canvas);
		void RemoveCanvas();
		void Render() const;
		void PostRender() const;

		void EnableEditorLayoutSave(bool p_value);
		bool IsEditorLayoutSaveEnabled() const;
		void SetEditorLayoutSaveFilename(const std::string& p_filename);
		void SetEditorLayoutAutosaveFrequency(float p_frequency);
		float GetEditorLayoutAutosaveFrequency();

		void LoadLayout(const std::string & p_fileName);
		void SaveLayout(const std::string & p_fileName);
		void SaveCurrentLayout();
		void SetLayout(const std::string & p_fileName);
		void DeleteLayout(const std::string & p_fileName);
		void RenameLayout(const std::string& p_fileName, const std::string& p_newFileName);

	private:
		void ApplyStyle();

	private:
		bool m_dockingState;
		Canvas* m_canvas;
		std::unordered_map<std::string, ImFont*> m_fonts;
		std::string m_layoutSaveFilename = "imgui.ini";
		const std::string m_defaultLayout;
		const std::string m_layoutsPath;
	};
}
