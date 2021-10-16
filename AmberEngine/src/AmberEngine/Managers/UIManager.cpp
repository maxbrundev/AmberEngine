#include "Amberpch.h"

#include <GLFW/glfw3.h>

#include "AmberEngine/Managers/UIManager.h"

AmberEngine::Manager::UIManager::UIManager(GLFWwindow* p_glfwWindow, const std::string& p_glslVersion)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
   //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(p_glfwWindow, true);
	ImGui_ImplOpenGL3_Init(p_glslVersion.c_str());
}

AmberEngine::Manager::UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void AmberEngine::Manager::UIManager::EnableDocking(bool p_value)
{
	m_dockingState = p_value;

	if (p_value)
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	else
		ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;
}

void AmberEngine::Manager::UIManager::PreDraw()
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
	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::SetWindowPos({ 0.f, 0.f });
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImGui::SetWindowSize({ (float)displaySize.x, (float)displaySize.y });
	ImGui::End();

	ImGui::PopStyleVar(3);
}

void AmberEngine::Manager::UIManager::PostDraw()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	
}