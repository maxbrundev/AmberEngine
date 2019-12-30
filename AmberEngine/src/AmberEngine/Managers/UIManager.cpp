#include "AmberEngine/Managers/UIManager.h"

#include "AmberEngine/ImGUI/imgui.h"
#include "AmberEngine/ImGUI/imgui_impl_glfw.h"
#include "AmberEngine/ImGUI/imgui_impl_opengl3.h"

AmberEngine::Managers::UIManager::UIManager(Context::Window& p_window) : m_window(p_window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(m_window.GetContextWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 12.0f;
}

AmberEngine::Managers::UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void AmberEngine::Managers::UIManager::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void AmberEngine::Managers::UIManager::EndFrame()
{
	ImGui::Render();
}

void AmberEngine::Managers::UIManager::BeginWindow(const std::string& p_title)
{
	ImGui::Begin(p_title.c_str());
}

void AmberEngine::Managers::UIManager::EndWindow()
{
	ImGui::End();
}

void AmberEngine::Managers::UIManager::Render()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void AmberEngine::Managers::UIManager::DisplayMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Quit"))
			{
				m_window.Close();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void AmberEngine::Managers::UIManager::DisplayDeviceInfos()
{
	/*ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoBackground;*/

	ImGui::Begin("Device");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
