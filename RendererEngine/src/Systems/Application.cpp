#include "pch.h"

#include "Systems/Application.h"
#include "Resources/AssimpModel.h"
#include "ImGui/imgui.h"

RenderEngine::Systems::Application::Application()
{
	m_renderingManager = std::make_unique<Managers::RenderingManager>();
}

void RenderEngine::Systems::Application::Setup()
{
	Resources::Shader& lightingShader = m_renderingManager->GetResourcesManager()->LoadShaderFiles("DirectionalLight", "directional_lighting.vs", "directional_lighting.fs");
	Resources::Texture& diffuseMap = m_renderingManager->GetResourcesManager()->LoadTexture("uvChecker", "Primitive/default.png");

	lightingShader.Bind();
	lightingShader.SetUniform1i("material.texture_diffuse1", 0);
	lightingShader.SetUniform1i("material.texture_specular1", 1);
	lightingShader.SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	lightingShader.SetUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingShader.SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.SetUniform1f("material.shininess", 30.0f);
	lightingShader.Unbind();
}

void RenderEngine::Systems::Application::Run()
{
	float UvX = 1.0f;
	float UvY = 1.0f;
	glm::vec3	lighDir = glm::vec3(-0.2f, -1.0f, -0.3f);

	Resources::AssimpModel model("res/Mesh/CornellBox/CornellBox-Glossy-Floor.obj");

	while (m_renderingManager->IsRunning())
	{
		m_renderingManager->Clear();
		m_renderingManager->Update();

		ImGui::Begin("Scene");
		ImGui::Text("Camera Position X: %.1f Y: %.1f Z: %.1f", m_renderingManager->GetCamera()->GetPosition().x, m_renderingManager->GetCamera()->GetPosition().y, m_renderingManager->GetCamera()->GetPosition().z);
		ImGui::SliderFloat("Light Direction X", &lighDir.x, -10.0f, 10.0f, "X: %.1f");
		ImGui::SliderFloat("Light Direction Y", &lighDir.y, -10.0f, 10.0f, "Y: %.1f");
		ImGui::SliderFloat("Light Direction Z", &lighDir.z, -10.0f, 10.0f, "Z: %.1f");
		ImGui::SliderFloat("Texture Tiling", &UvX, -10.0f, 10.0f, "X");
		ImGui::SliderFloat("Texture Tiling", &UvY, -10.0f, 10.0f, "Y");
		ImGui::End();

		glm::mat4 projectionMatrix = m_renderingManager->CalculateProjectionMatrix();
		glm::mat4 viewMatrix = m_renderingManager->CalculateViewMatrix();
		glm::mat4 modelMatrix = m_renderingManager->GetModelMatrix();
		
		m_renderingManager->GetResourcesManager()->GetShader("DirectionalLight").Bind();
		m_renderingManager->GetResourcesManager()->GetShader("DirectionalLight").SetUniformMat4("projection", projectionMatrix);
		m_renderingManager->GetResourcesManager()->GetShader("DirectionalLight").SetUniformMat4("view", viewMatrix);
		m_renderingManager->GetResourcesManager()->GetShader("DirectionalLight").SetUniformMat4("model", modelMatrix);
		m_renderingManager->GetResourcesManager()->GetShader("DirectionalLight").SetUniformVec3("viewPos", m_renderingManager->GetCamera()->GetPosition());
		m_renderingManager->GetResourcesManager()->GetShader("DirectionalLight").SetUniformVec3("light.direction", lighDir);
		m_renderingManager->GetResourcesManager()->GetShader("DirectionalLight").SetUniform1f("UvXValue", UvX);
		m_renderingManager->GetResourcesManager()->GetShader("DirectionalLight").SetUniform1f("UvYValue", UvY);
		m_renderingManager->GetResourcesManager()->GetTexture("uvChecker").Bind();

		model.Draw();
		
		m_renderingManager->SwapBuffers();
	}
}