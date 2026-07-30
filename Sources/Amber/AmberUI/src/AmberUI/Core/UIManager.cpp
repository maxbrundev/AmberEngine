#include "Amberpch.h"

#include "AmberUI/Core/UIManager.h"

#include "AmberUI/Canvas.h"

AmberUI::Core::UIManager::UIManager(GLFWwindow* p_glfwWindow, const std::string& p_glslVersion) : m_dockingState(false), m_defaultLayout("Config\\layout.ini"), m_layoutsPath(std::string(getenv("APPDATA")) + "\\AmberEngine\\Editor\\Layouts\\"), m_defaultLayoutSaveFilename(std::string(getenv("APPDATA")) + "\\AmberEngine\\Editor\\layout.ini")
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	auto& IO = ImGui::GetIO();
	IO.ConfigWindowsMoveFromTitleBarOnly = true; /* Disable moving windows by dragging another thing than the title bar */

	EnableDocking(false);

	ApplyStyle();

	ImGui_ImplGlfw_InitForOpenGL(p_glfwWindow, true);
	ImGui_ImplOpenGL3_Init(p_glslVersion.c_str());

	std::filesystem::create_directories(m_layoutsPath);
}

AmberUI::Core::UIManager::~UIManager()
{
	m_fonts.clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool AmberUI::Core::UIManager::LoadFont(const std::string& p_id, const std::string& p_path, float p_fontSize)
{
	if (m_fonts.find(p_id) == m_fonts.end())
	{
		auto& io = ImGui::GetIO();
		ImFont* fontInstance = io.Fonts->AddFontFromFileTTF(p_path.c_str(), p_fontSize);

		if (fontInstance)
		{
			m_fonts[p_id] = fontInstance;
			return true;
		}
	}

	return false;
}

bool AmberUI::Core::UIManager::UnloadFont(const std::string& p_id)
{
	if (m_fonts.find(p_id) != m_fonts.end())
	{
		m_fonts.erase(p_id);
		return true;
	}

	return false;
}

bool AmberUI::Core::UIManager::UseFont(const std::string& p_id)
{
	auto foundFont = m_fonts.find(p_id);

	if (foundFont != m_fonts.end())
	{
		ImGui::GetIO().FontDefault = foundFont->second;
		return true;
	}

	return false;
}

void AmberUI::Core::UIManager::UseDefaultFont()
{
	ImGui::GetIO().FontDefault = nullptr;
}

void AmberUI::Core::UIManager::EnableDocking(bool p_value) 
{
	m_dockingState = p_value;

	if (p_value)
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	else
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;
}

void AmberUI::Core::UIManager::SetCanvas(AmberUI::Canvas& p_canvas)
{
	RemoveCanvas();
	m_canvas = &p_canvas;
}

void AmberUI::Core::UIManager::RemoveCanvas()
{
	m_canvas = nullptr;
}

void AmberUI::Core::UIManager::Render() const
{
	if (m_canvas)
	{
		m_canvas->Draw();
	}
}

void AmberUI::Core::UIManager::PostRender() const
{
	if (m_canvas)
	{
		m_canvas->PostDraw();
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void AmberUI::Core::UIManager::EnableEditorLayoutSave(bool p_value)
{
	if (p_value)
		ImGui::GetIO().IniFilename = m_layoutSaveFilename.c_str();
	else
		ImGui::GetIO().IniFilename = nullptr;
}

bool AmberUI::Core::UIManager::IsEditorLayoutSaveEnabled() const
{
	return ImGui::GetIO().IniFilename != nullptr;
}

void AmberUI::Core::UIManager::SetEditorLayoutSaveFilename(const std::string& p_filename)
{
	m_layoutSaveFilename = p_filename;

	if (IsEditorLayoutSaveEnabled())
		ImGui::GetIO().IniFilename = m_layoutSaveFilename.c_str();
}

void AmberUI::Core::UIManager::SetEditorLayoutAutosaveFrequency(float p_frequency)
{
	ImGui::GetIO().IniSavingRate = p_frequency;
}

float AmberUI::Core::UIManager::GetEditorLayoutAutosaveFrequency()
{
	return ImGui::GetIO().IniSavingRate; 
}

void AmberUI::Core::UIManager::LoadConfigLayoutSettings() const
{
	ImGui::LoadIniSettingsFromDisk(m_defaultLayout.c_str());
}

void AmberUI::Core::UIManager::SaveLayout(const std::string& p_fileName)
{
	SetEditorLayoutSaveFilename(p_fileName);

	ImGui::SaveIniSettingsToDisk(m_layoutSaveFilename.c_str());
}

void AmberUI::Core::UIManager::SaveCurrentLayout()
{
	if(!std::filesystem::exists(m_layoutSaveFilename))
	{
		SetEditorLayoutSaveFilename(m_defaultLayoutSaveFilename);
	}
	ImGui::SaveIniSettingsToDisk(m_layoutSaveFilename.c_str());
}

void AmberUI::Core::UIManager::SetDefaultLayout()
{
	SetEditorLayoutSaveFilename(m_defaultLayoutSaveFilename);

	if (std::filesystem::exists(m_defaultLayoutSaveFilename))
	{
		ImGui::LoadIniSettingsFromDisk(m_defaultLayoutSaveFilename.c_str());
	}
	else
	{
		LoadConfigLayoutSettings();
	}
}

void AmberUI::Core::UIManager::SetIniLayout(const std::string& p_fileName)
{
	const std::string iniLayoutPath = m_layoutsPath + p_fileName + ".ini";

	if(std::filesystem::exists(iniLayoutPath))
	{
		SetLayout(iniLayoutPath);
	}
}

void AmberUI::Core::UIManager::SetLayout(const std::string& p_fileName)
{
	SetEditorLayoutSaveFilename(p_fileName);

	ImGui::LoadIniSettingsFromDisk(p_fileName.c_str());
}

void AmberUI::Core::UIManager::DeleteLayout(const std::string& p_fileName)
{
	std::filesystem::remove(p_fileName);

	if(m_layoutSaveFilename == p_fileName)
	{
		SetEditorLayoutSaveFilename(m_defaultLayoutSaveFilename);
	}
}

void AmberUI::Core::UIManager::RenameLayout(const std::string& p_fileName, const std::string& p_newFileName)
{
	std::filesystem::rename(p_fileName, p_newFileName);

	if(m_layoutSaveFilename == p_fileName)
	{
		SetEditorLayoutSaveFilename(p_newFileName);
	}
}

const std::string& AmberUI::Core::UIManager::GetLayoutsPath() const
{
	return m_layoutsPath;
}

void AmberUI::Core::UIManager::ApplyStyle()
{
	ImGui::StyleColorsDark();

	ImGuiStyle* style = &ImGui::GetStyle();

	style->Alpha = 0.95f;
	style->WindowMinSize = ImVec2(160, 20);
	style->WindowRounding = 0.0f;
	style->ChildRounding = 0.0f;
	style->PopupRounding = 0.0f;
	style->FramePadding = ImVec2(4, 2);
	style->FrameRounding = 0.0f;
	style->ItemSpacing = ImVec2(6, 2);
	style->ItemInnerSpacing = ImVec2(2, 4);
	style->IndentSpacing = 6.0f;
	style->ColumnsMinSpacing = 50.0f;
	style->ScrollbarSize = 12.0f;
	style->ScrollbarRounding = 0.0f;
	style->GrabRounding = 0.0f;
	style->TabRounding = 0.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.90f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.31f, 0.34f, 0.41f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.48f, 0.52f, 0.60f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.31f, 0.34f, 0.41f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.31f, 0.34f, 0.41f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.48f, 0.52f, 0.60f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.35f, 0.85f, 0.47f, 1.00f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.31f, 0.34f, 0.41f, 1.00f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.48f, 0.52f, 0.60f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.31f, 0.34f, 0.41f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.48f, 0.52f, 0.60f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(1.00f, 1.00f, 1.00f, 0.16f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(1.00f, 1.00f, 1.00f, 0.24f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(1.00f, 1.00f, 1.00f, 0.32f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.31f, 0.34f, 0.41f, 1.00f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.48f, 0.52f, 0.60f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.31f, 0.34f, 0.41f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.48f, 0.52f, 0.60f, 1.00f);
	style->Colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style->Colors[ImGuiCol_TabHovered] = ImVec4(0.48f, 0.52f, 0.60f, 1.00f);
	style->Colors[ImGuiCol_TabActive] = ImVec4(0.31f, 0.34f, 0.41f, 1.00f);
	style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
	style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
	style->Colors[ImGuiCol_DockingPreview] = ImVec4(0.48f, 0.52f, 0.60f, 0.70f);
	style->Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.35f, 0.85f, 0.47f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.35f, 0.85f, 0.47f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.35f, 0.85f, 0.47f, 0.43f);
	style->Colors[ImGuiCol_DragDropTarget] = ImVec4(0.35f, 0.85f, 0.47f, 0.90f);
	style->Colors[ImGuiCol_NavHighlight] = ImVec4(0.48f, 0.52f, 0.60f, 1.00f);
	style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
}
