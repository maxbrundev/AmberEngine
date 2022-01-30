#include "Amberpch.h"

#include "AmberEngine/Core/UIManager.h"

AmberEngine::Core::UIManager::UIManager(GLFWwindow* p_glfwWindow, const std::string& p_glslVersion)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true; /* Disable moving windows by dragging another thing than the title bar */
	EnableDocking(false);
	ImGui_ImplGlfw_InitForOpenGL(p_glfwWindow, true);
	ImGui_ImplOpenGL3_Init(p_glslVersion.c_str());
}

AmberEngine::Core::UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void AmberEngine::Core::UIManager::EnableDocking(bool p_value)
{
	m_dockingState = p_value;

	if (p_value)
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	else
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;
}

void AmberEngine::Core::UIManager::PreDraw()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("##dockspace", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
	const ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::SetWindowPos({ 0.0f, 0.0f });
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetWindowSize({ displaySize.x, displaySize.y });
	ImGui::End();

	ImGui::PopStyleVar(3);
}

void AmberEngine::Core::UIManager::PostDraw()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}