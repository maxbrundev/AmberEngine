#include "pch.h"

#include "Systems/Application.h"
#include "ImGui/imgui.h"

#include "World.h"

RenderEngine::Systems::Application::Application()
{
	m_renderingManager = std::make_unique<Managers::RenderingManager>();
}

void RenderEngine::Systems::Application::Setup()
{
	Resources::Shader& chunkShader = m_renderingManager->GetResourcesManager()->LoadShaderFiles("chunk", "chunk.vs", "chunk.fs");
	Resources::Texture& texture = m_renderingManager->GetResourcesManager()->LoadTexture("dirt", "planks_oak.png");
}

void RenderEngine::Systems::Application::Run()
{
	AmberCraft::World world;

	while (m_renderingManager->IsRunning())
	{
		m_renderingManager->Clear();
		m_renderingManager->Update();

		ImGui::Begin("Scene");
		ImGui::Text("Camera Position X: %.1f Y: %.1f Z: %.1f", m_renderingManager->GetCamera()->GetPosition().x, m_renderingManager->GetCamera()->GetPosition().y, m_renderingManager->GetCamera()->GetPosition().z);
		ImGui::End();

		world.Draw(*m_renderingManager);

		m_renderingManager->SwapBuffers();
	}
}