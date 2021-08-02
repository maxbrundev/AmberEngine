#include "Amberpch.h"

#include "AmberEngine/Core/UIManager.h"

AmberEngine::Core::UIManager::UIManager(Context::Window& p_window) : m_window(p_window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(m_window.GetGlfwWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 12.0f;

	//ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

AmberEngine::Core::UIManager::~UIManager()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void AmberEngine::Core::UIManager::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void AmberEngine::Core::UIManager::EndFrame()
{
	ImGui::Render();
}

void AmberEngine::Core::UIManager::BeginWindow(const std::string& p_title)
{
	ImGui::Begin(p_title.c_str());
}

void AmberEngine::Core::UIManager::EndWindow()
{
	ImGui::End();
}

void AmberEngine::Core::UIManager::Render()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void AmberEngine::Core::UIManager::DisplayMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if (ImGui::MenuItem("Quit"))
			{
				m_window.SetShouldClose(true);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void AmberEngine::Core::UIManager::DisplayDeviceInfos()
{
	ImGui::Begin("Device");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
