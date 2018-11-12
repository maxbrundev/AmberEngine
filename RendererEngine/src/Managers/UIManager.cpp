#include "pch.h"

#include "Managers/UIManager.h"

RenderEngine::Managers::UIManager::UIManager(Core::Device& p_context) : m_device(p_context), m_clearColor{ 0.1f, 0.1f, 0.1f, 1.0f }
{
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(m_device.GetContextWindow(), true);
	ImGui::StyleColorsDark();
}

void RenderEngine::Managers::UIManager::PreUpdate()
{
	ImGui_ImplGlfwGL3_NewFrame();
	GLCall(glClearColor(m_clearColor[0], m_clearColor[1], m_clearColor[2], m_clearColor[3]));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void RenderEngine::Managers::UIManager::Update(LowRenderer::Camera& p_camera)
{
	ImGui::Text("Scene");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Bullet();
	ImGui::Text("Camera Transform \nPosition X: %f Y: %f Z: %f", p_camera.GetPosition().x, p_camera.GetPosition().y, p_camera.GetPosition().z);

	ImGui::ColorEdit4("Clear Color", m_clearColor);
}

void RenderEngine::Managers::UIManager::PostUpdate()
{
	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void RenderEngine::Managers::UIManager::Close()
{
	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

void RenderEngine::Managers::UIManager::DisplayTexture(Resources::Texture p_texture)
{
	ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(p_texture.GetTextureId())), ImVec2(p_texture.GetTextureWidth(), p_texture.GetTextureHeight()));
}