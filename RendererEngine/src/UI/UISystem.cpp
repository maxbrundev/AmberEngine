#include "pch.h"

#include "UI/UISystem.h"

RenderEngine::Systems::UISystem::UISystem(Core::Context& p_context) : m_context(p_context)
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(m_context.GetContextWindow(), true);
	ImGui::StyleColorsDark();
}

void RenderEngine::Systems::UISystem::PreUpdate()
{
	ImGui_ImplGlfwGL3_NewFrame();
}

void RenderEngine::Systems::UISystem::Update(unsigned int p_drawcalls, LowRenderer::Camera& p_camera)
{
	ImGui::Text("Scene");
	ImGui::Text("DrawCalls/frame: %d", p_drawcalls);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Bullet();
	ImGui::Text("Camera Transform \nPosition X: %f Y: %f Z: %f", p_camera.m_position.x, p_camera.m_position.y, p_camera.m_position.z);
}

void RenderEngine::Systems::UISystem::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void RenderEngine::Systems::UISystem::Close()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}