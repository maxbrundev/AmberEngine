#include "pch.h"

#include "Application.h"

#include <AmberEngine/Resources/Shader.h>
#include <AmberEngine/Resources/AssimpModel.h>
#include <AmberEngine/Managers/UIManager.h>
#include <AmberEngine/ImGUI/imgui.h>

Example::Application::Application(const AmberEngine::Settings::RenderingSettings& p_settings) : m_renderer(p_settings)
{
}

void Example::Application::Setup()
{
	AmberEngine::Resources::Shader& lightingShader = m_renderer.GetResourcesManager().LoadShaderFiles("DirectionalLight", "directional_lighting.vs", "directional_lighting.fs");

	lightingShader.Bind();
	lightingShader.SetUniform1i("material.texture_diffuse1", 0);
	lightingShader.SetUniform1i("material.texture_specular1", 1);
	lightingShader.SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	lightingShader.SetUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingShader.SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.SetUniform1f("material.shininess", 30.0f);
	lightingShader.Unbind();
}

void Example::Application::Run()
{
	float UvX = 1.0f;
	float UvY = 1.0f;
	glm::vec3 lighDir = glm::vec3(-0.2f, -1.0f, -0.3f);

	AmberEngine::Resources::AssimpModel model("res/Mesh/nanosuit/nanosuit.obj");

	AmberEngine::Managers::UIManager ui(m_renderer.GetWindowManager().GetDevice());

	while (m_renderer.IsRunning())
	{
		m_renderer.Clear(0.1f, 0.1f, 0.1f);
		m_renderer.Update();

		glm::vec3 cameraPosition = m_renderer.GetCameraController().GetCamera().GetPosition();

		ui.BeginFrame();

		ui.DisplayDeviceInfos();
		ui.DisplayMenu();
		ui.BeginWindow("Scene");
		ImGui::Text("Camera Position X: %.1f Y: %.1f Z: %.1f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
		ImGui::DragFloat("Light Direction X", &lighDir.x, 0.005f, 0.0f, 0.0f, "X: %.1f");
		ImGui::DragFloat("Light Direction Y", &lighDir.y, 0.005f, 0.0f, 0.0f, "Y: %.1f");
		ImGui::DragFloat("Light Direction Z", &lighDir.z, 0.005f, 0.0f, 0.0f, "Z: %.1f");

		ImGui::SliderFloat("Texture Tiling", &UvX, -10.0f, 10.0f, "X");
		ImGui::SliderFloat("Texture Tiling", &UvY, -10.0f, 10.0f, "Y");
		ui.EndWindow();
		//ImGui::ShowDemoWindow();
		glm::mat4 projectionMatrix = m_renderer.CalculateProjectionMatrix();
		glm::mat4 viewMatrix = m_renderer.CalculateViewMatrix();
		glm::mat4 modelMatrix = m_renderer.GetUnitModelMatrix();

		m_renderer.GetResourcesManager().GetShader("DirectionalLight").Bind();
		m_renderer.GetResourcesManager().GetShader("DirectionalLight").SetUniformMat4("projection", projectionMatrix);
		m_renderer.GetResourcesManager().GetShader("DirectionalLight").SetUniformMat4("view", viewMatrix);
		m_renderer.GetResourcesManager().GetShader("DirectionalLight").SetUniformMat4("model", modelMatrix);
		m_renderer.GetResourcesManager().GetShader("DirectionalLight").SetUniformVec3("viewPos", cameraPosition);
		m_renderer.GetResourcesManager().GetShader("DirectionalLight").SetUniformVec3("light.direction", lighDir);
		m_renderer.GetResourcesManager().GetShader("DirectionalLight").SetUniform1f("UvXValue", UvX);
		m_renderer.GetResourcesManager().GetShader("DirectionalLight").SetUniform1f("UvYValue", UvY);

		model.Draw();

		ui.EndFrame();
		ui.Render();

		m_renderer.SwapBuffers();
	}
}