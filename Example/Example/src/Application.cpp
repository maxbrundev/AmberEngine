#include "pch.h"

#include "Application.h"

#include <AmberEngine/Tools/Time/Clock.h>
#include <AmberEngine/ImGUI/imgui.h>

Example::Application::Application(const AmberEngine::Settings::DeviceSettings & p_deviceSettings, 
	const AmberEngine::Settings::WindowSettings & p_windowSettings, 
	const AmberEngine::Settings::DriverSettings & p_driverSettings) : 
	m_context(p_deviceSettings, p_windowSettings, p_driverSettings),
	m_editor(m_context),
	ui(*m_context.m_window)
{
	isRunning = true;
}

void Example::Application::Setup()
{
	m_context.m_resourcesManager.LoadModel("Suit", "res/Mesh/nanosuit/nanosuit.obj");

	AmberEngine::Resources::Shader& lightingShader = m_context.m_resourcesManager.LoadShader("StandardLighting", "res/shaders/StandardLighting.glsl");
	
	m_context.m_resourcesManager.LoadTexture("diffuse", "crystal.jpg");
	m_context.m_resourcesManager.LoadTexture("specular", "crystal_spec.jpg");
	m_context.m_resourcesManager.GetModel("Suit").SetShader(lightingShader);
	lightingShader.Bind();
	lightingShader.SetUniform1i("u_DiffuseMap", 0);
	lightingShader.SetUniform1i("u_SpecularMap", 1);
	lightingShader.SetUniformVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	lightingShader.SetUniformVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	lightingShader.SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	lightingShader.Unbind();
}

void Example::Application::Run()
{
	glm::vec3 lighDir = glm::vec3(1, 1, 1);

	Utils::Clock clock;

	while (IsRunning())
	{
		m_editor.PreUpdate();

		glm::vec3 cameraPosition = m_editor.GetCameraController().GetPosition();

		glm::mat4 projectionMatrix = m_editor.GetCameraController().GetCamera().GetProjectionMatrix();
		glm::mat4 viewMatrix = m_editor.GetCameraController().GetCamera().GetViewMatrix();
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		auto& shader = m_context.m_resourcesManager.GetShader("StandardLighting");
		
		
		shader.Bind();
		shader.SetUniformMat4("projection", projectionMatrix);
		shader.SetUniformMat4("view", viewMatrix);
		shader.SetUniformMat4("model", modelMatrix);
		shader.SetUniformVec3("viewPos", cameraPosition);
		shader.SetUniformVec3("light.direction", lighDir);
		
		m_context.m_resourcesManager.GetTexture("diffuse").Bind();
		m_context.m_resourcesManager.GetTexture("specular").Bind(1);

		m_context.m_renderer->Draw(m_context.m_resourcesManager.GetModel("Suit"));

		shader.Unbind();
		m_editor.Update(clock.GetDeltaTime());
		m_editor.PostUpdate();
		clock.Update();
	}
}

bool Example::Application::IsRunning()
{
	return isRunning && m_context.m_window->IsActive();
}
