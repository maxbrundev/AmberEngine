#include "pch.h"

#include "Managers/UIManager.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

RenderEngine::Managers::UIManager::UIManager(Core::Device& p_context) : m_device(p_context), m_clearColor{ 0.1f, 0.1f, 0.1f, 1.0f }
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(m_device.GetContextWindow(), true);
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameRounding = 12.0f;
}

void RenderEngine::Managers::UIManager::PreUpdate()
{
	ImGui_ImplGlfwGL3_NewFrame();
	GLCall(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderEngine::Managers::UIManager::Update(LowRenderer::Camera& p_camera)
{
	DisplayMenuBar();
	ImGui::Begin("Scene");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Camera Position X: %.1f Y: %.1f Z: %.1f", p_camera.GetPosition().x, p_camera.GetPosition().y, p_camera.GetPosition().z);
	ImGui::ColorEdit4("Clear Color", m_clearColor);
	ImGui::End();
}

void RenderEngine::Managers::UIManager::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void RenderEngine::Managers::UIManager::DisplayMenuBar()
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

void RenderEngine::Managers::UIManager::Close()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}
