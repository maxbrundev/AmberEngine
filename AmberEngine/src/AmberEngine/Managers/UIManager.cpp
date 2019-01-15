#include "AmberEngine/Debug/GLDebug.h"
#include "AmberEngine/Managers/UIManager.h"


#include "AmberEngine/ImGui/imgui.h"
#include "AmberEngine/ImGui/imgui_impl_glfw_gl3.h"

AmberEngine::Managers::UIManager::UIManager(Core::Device& p_context) : m_device(p_context), m_clearColor{ 0.1f, 0.1f, 0.1f, 1.0f }
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(m_device.GetContextWindow(), true);
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 12.0f;
}

AmberEngine::Managers::UIManager::~UIManager()
{
	Close();
}

void AmberEngine::Managers::UIManager::PreUpdate()
{
	ImGui_ImplGlfwGL3_NewFrame();
	GLCall(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void AmberEngine::Managers::UIManager::Update()
{
	DisplayMenuBar();
	ImGui::Begin("Scene");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::ColorEdit4("Clear Color", m_clearColor);
	ImGui::End();
}

void AmberEngine::Managers::UIManager::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void AmberEngine::Managers::UIManager::DisplayMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			if(ImGui::MenuItem("Quit"))
			{
				m_device.Close();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void AmberEngine::Managers::UIManager::Close()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}
