#include "Amberpch.h"

#include "AmberEngine/UI/Core/UIManager.h"

#include "AmberEngine/UI/Canvas.h"

AmberEngine::UI::Core::UIManager::UIManager(GLFWwindow* p_glfwWindow, const std::string& p_glslVersion) : m_dockingState(false), m_defaultLayout("Config\\layout.ini"), m_layoutsPath(std::string(getenv("APPDATA")) + "\\AmberEngine\\Editor\\")
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	auto& IO = ImGui::GetIO();
	IO.ConfigWindowsMoveFromTitleBarOnly = true; /* Disable moving windows by dragging another thing than the title bar */

	EnableDocking(false);

	ApplyStyle();

	ImGui_ImplGlfw_InitForOpenGL(p_glfwWindow, true);
	ImGui_ImplOpenGL3_Init(p_glslVersion.c_str());
}

AmberEngine::UI::Core::UIManager::~UIManager()
{
	m_fonts.clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool AmberEngine::UI::Core::UIManager::LoadFont(const std::string& p_id, const std::string& p_path, float p_fontSize)
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

bool AmberEngine::UI::Core::UIManager::UnloadFont(const std::string& p_id)
{
	if (m_fonts.find(p_id) != m_fonts.end())
	{
		m_fonts.erase(p_id);
		return true;
	}

	return false;
}

bool AmberEngine::UI::Core::UIManager::UseFont(const std::string& p_id)
{
	auto foundFont = m_fonts.find(p_id);

	if (foundFont != m_fonts.end())
	{
		ImGui::GetIO().FontDefault = foundFont->second;
		return true;
	}

	return false;
}

void AmberEngine::UI::Core::UIManager::UseDefaultFont()
{
	ImGui::GetIO().FontDefault = nullptr;
}

void AmberEngine::UI::Core::UIManager::EnableDocking(bool p_value) 
{
	m_dockingState = p_value;

	if (p_value)
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	else
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;
}

void AmberEngine::UI::Core::UIManager::SetCanvas(UI::Canvas& p_canvas)
{
	RemoveCanvas();
	m_canvas = &p_canvas;
}

void AmberEngine::UI::Core::UIManager::RemoveCanvas()
{
	m_canvas = nullptr;
}

void AmberEngine::UI::Core::UIManager::Render() const
{
	if (m_canvas)
	{
		m_canvas->Draw();
	}
}

void AmberEngine::UI::Core::UIManager::PostRender() const
{
	if (m_canvas)
	{
		m_canvas->PostDraw();
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void AmberEngine::UI::Core::UIManager::EnableEditorLayoutSave(bool p_value)
{
	if (p_value)
		ImGui::GetIO().IniFilename = m_layoutSaveFilename.c_str();
	else
		ImGui::GetIO().IniFilename = nullptr;
}

bool AmberEngine::UI::Core::UIManager::IsEditorLayoutSaveEnabled() const
{
	return ImGui::GetIO().IniFilename != nullptr;
}

void AmberEngine::UI::Core::UIManager::SetEditorLayoutSaveFilename(const std::string& p_filename)
{
	m_layoutSaveFilename = p_filename;

	if (IsEditorLayoutSaveEnabled())
		ImGui::GetIO().IniFilename = m_layoutSaveFilename.c_str();
}

void AmberEngine::UI::Core::UIManager::SetEditorLayoutAutosaveFrequency(float p_frequency)
{
	ImGui::GetIO().IniSavingRate = p_frequency;
}

float AmberEngine::UI::Core::UIManager::GetEditorLayoutAutosaveFrequency()
{
	return ImGui::GetIO().IniSavingRate; 
}

void AmberEngine::UI::Core::UIManager::LoadLayout(const std::string& p_fileName)
{
	ImGui::LoadIniSettingsFromDisk(p_fileName.c_str());
}

void AmberEngine::UI::Core::UIManager::SaveLayout(const std::string& p_fileName)
{
	SetEditorLayoutSaveFilename(p_fileName);

	ImGui::SaveIniSettingsToDisk(m_layoutSaveFilename.c_str());
}

void AmberEngine::UI::Core::UIManager::SaveCurrentLayout()
{
	if(!std::filesystem::exists(m_layoutSaveFilename))
	{
		m_layoutSaveFilename = m_layoutsPath + "layout.ini";
		SetEditorLayoutSaveFilename(m_layoutSaveFilename);
	}
	ImGui::SaveIniSettingsToDisk(m_layoutSaveFilename.c_str());
}

void AmberEngine::UI::Core::UIManager::SetLayout(const std::string& p_fileName)
{
	SetEditorLayoutSaveFilename(p_fileName);

	ImGui::LoadIniSettingsFromDisk(p_fileName.c_str());
}

void AmberEngine::UI::Core::UIManager::DeleteLayout(const std::string& p_fileName)
{
	std::filesystem::remove(p_fileName);
}

void AmberEngine::UI::Core::UIManager::RenameLayout(const std::string& p_fileName, const std::string& p_newFileName)
{
	std::filesystem::rename(p_fileName, p_newFileName);

	if(m_layoutSaveFilename == p_fileName)
	{
		SetEditorLayoutSaveFilename(p_newFileName);
	}
}

void AmberEngine::UI::Core::UIManager::ApplyStyle()
{
	ImGui::StyleColorsDark();

	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowPadding = ImVec2(15, 15);
	style->WindowRounding = 0.0f;
	style->WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style->FramePadding = ImVec2(5, 5);
	style->FrameRounding = 0.0f;
	style->ItemSpacing = ImVec2(12, 8);
	style->ItemInnerSpacing = ImVec2(8, 6);
	style->IndentSpacing = 25.0f;
	style->ScrollbarSize = 15.0f;
	style->ScrollbarRounding = 0.0f;
	style->GrabMinSize = 5.0f;
	style->GrabRounding = 0.0f;
	style->ChildRounding = 0.0f;
	style->TabRounding = 0.0f;
	style->PopupRounding = 0.0f;
	style->ChildBorderSize = 1.0f;
	style->WindowBorderSize = 1.0f;
	style->PopupBorderSize = 1.0f;

	style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.2f, 0.88f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.3f, 0.3f, 0.3f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
	style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
	style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

	style->Colors[ImGuiCol_Tab] = style->Colors[ImGuiCol_TabUnfocused];
}
