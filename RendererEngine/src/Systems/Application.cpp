#include "pch.h"

#include "Systems/Application.h"
#include "Resources/Mesh.h"
#include "ImGui/imgui.h"

RenderEngine::Systems::Application::Application()
{
	m_renderingManager = std::make_unique<Managers::RenderingManager>();
}

void RenderEngine::Systems::Application::Setup()
{
	Resources::Shader& lightingShader = m_renderingManager->GetResourcesManager()->LoadShaderFiles("DirectionalLight", "directional_lighting.vs", "directional_lighting.fs");
	
	Resources::Texture& diffuseMap = m_renderingManager->GetResourcesManager()->LoadTexture("diffuse", "crystal.jpg");
	Resources::Texture& specularMap = m_renderingManager->GetResourcesManager()->LoadTexture("specular", "crystal_spec.jpg");

	lightingShader.Bind();
	lightingShader.SetUniform1i("material.diffuse", 0);
	lightingShader.SetUniform1i("material.specular", 1);
	lightingShader.SetUniformVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	lightingShader.SetUniformVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.SetUniform1f("material.shininess", 10.0f);
	lightingShader.Unbind();
}

void RenderEngine::Systems::Application::Run()
{
	Resources::Mesh model("res/Mesh/monkey3.obj");

	glm::vec3 cubePositions[] = {
	   glm::vec3(0.0f,  0.0f,  0.0f),
	   glm::vec3(2.0f,  3.0f, -8.0f),
	   glm::vec3(-1.5f, -2.2f, -2.5f),
	   glm::vec3(-2.8f, -2.0f, -9.3f),
	   glm::vec3(2.4f, -0.4f, -3.5f),
	   glm::vec3(-1.7f,  3.0f, -6.5f),
	   glm::vec3(1.3f, -2.0f, -2.5f),
	   glm::vec3(1.5f,  2.0f, -2.5f),
	   glm::vec3(1.5f,  0.2f, -1.5f),
	   glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 lighDir = glm::vec3(-0.2f, -1.0f, -0.3f);

	while (m_renderingManager->IsRunning())
	{
		m_renderingManager->Clear();
		m_renderingManager->Update();

		ImGui::Begin("Scene");
		ImGui::SliderFloat("Light Direction X", &lighDir.x, -10.0f, 10.0f, "X: %.1f");
		ImGui::SliderFloat("Light Direction Y", &lighDir.y, -10.0f, 10.0f, "Y: %.1f");
		ImGui::SliderFloat("Light Direction Z", &lighDir.z, -10.0f, 10.0f, "Z: %.1f");
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

		m_renderingManager->GetResourcesManager()->GetTexture("diffuse").Bind();
		m_renderingManager->GetResourcesManager()->GetTexture("specular").Bind(1);

		for (unsigned int i = 0; i < 10; i++)
		{
			modelMatrix = glm::translate(modelMatrix, cubePositions[i]);
			float angle = 20.0f * i;
			modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_renderingManager->GetResourcesManager()->GetShader("DirectionalLight").SetUniformMat4("model", modelMatrix);

			model.Draw();
		}

		m_renderingManager->SwapBuffers();
	}
}