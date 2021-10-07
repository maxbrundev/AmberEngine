#include "pch.h"

#include "Application.h"

#include <AmberEngine/Tools/Time/Clock.h>
#include <AmberEngine/ImGUI/imgui.h>
#include <AmberEngine/Resources/Primitives/Cube.h>
#include <AmberEngine/Buffers/VertexBuffer.h>
#include <AmberEngine/Buffers/VertexArray.h>

Example::Application::Application(const AmberEngine::Settings::DeviceSettings & p_deviceSettings,
	const AmberEngine::Settings::WindowSettings & p_windowSettings,
	const AmberEngine::Settings::DriverSettings & p_driverSettings) :
	m_context(p_deviceSettings, p_windowSettings, p_driverSettings),
	m_editor(m_context),
	m_uiManager(*m_context.m_window)
{
	isRunning = true;
}

void Example::Application::Setup()
{
	m_context.m_resourcesManager.LoadModel("Suit", "res/Mesh/nanosuit/nanosuit.obj");

	AmberEngine::Resources::Shader& lightingShader = m_context.m_resourcesManager.LoadShader("StandardLighting", "res/shaders/StandardLighting.glsl");
	
	m_context.m_resourcesManager.LoadTexture("diffuse", "crystal.jpg", AmberEngine::Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::DIFFUSE,true, true);
	m_context.m_resourcesManager.LoadTexture("specular", "crystal_spec.jpg", AmberEngine::Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::DIFFUSE, true, true);
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
	glm::vec3 lighDir = glm::vec3(1.0f, 1.0f, 1.0f);

	Utils::Clock clock;
	/*AmberEngine::PrimitivesShapes::Cube::Setup();
	
	std::vector<float> vertices;
	std::vector<AmberEngine::PrimitivesShapes::Vertex> verticesCube = AmberEngine::PrimitivesShapes::Cube::GetVertices();
	
	for (auto& vertex : verticesCube)
	{
		vertices.push_back(vertex.position[0]);
		vertices.push_back(vertex.position[1]);
		vertices.push_back(vertex.position[2]);
		vertices.push_back(vertex.textureCoord[0]);
		vertices.push_back(vertex.textureCoord[1]);
		vertices.push_back(vertex.normals[0]);
		vertices.push_back(vertex.normals[1]);
		vertices.push_back(vertex.normals[2]);
	}
	
	AmberEngine::Buffers::VertexArray vao;
	AmberEngine::Buffers::VertexBuffer vbo(vertices.data(), vertices.size() * sizeof(vertices));
	vbo.Bind();
	vao.Bind();
	vao.BindAttribPointer(3, GL_FLOAT, GL_FALSE, sizeof(AmberEngine::PrimitivesShapes::Vertex), nullptr);
	vao.BindAttribPointer(2, GL_FLOAT, GL_FALSE, sizeof(AmberEngine::PrimitivesShapes::Vertex), reinterpret_cast<void*>(offsetof(AmberEngine::PrimitivesShapes::Vertex, AmberEngine::PrimitivesShapes::Vertex::textureCoord)));
	vao.BindAttribPointer(3, GL_FLOAT, GL_FALSE, sizeof(AmberEngine::PrimitivesShapes::Vertex), reinterpret_cast<void*>(offsetof(AmberEngine::PrimitivesShapes::Vertex, AmberEngine::PrimitivesShapes::Vertex::normals)));
	vao.Unbind();
	vbo.Unbind();*/

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
		
		//m_context.m_resourcesManager.GetTexture("diffuse").Bind();
		//m_context.m_resourcesManager.GetTexture("specular").Bind(1);

		m_context.m_renderer->Draw(m_context.m_resourcesManager.GetModel("Suit"));
		//vao.Bind();
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//vao.Unbind();
		shader.Unbind();
		
		m_uiManager.BeginFrame();
		m_uiManager.DisplayDeviceInfos();

		m_uiManager.BeginWindow("Scene");
		ImGui::Text("Camera Position X: %.1f Y: %.1f Z: %.1f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
		ImGui::DragFloat("Light Direction X", &lighDir.x, 0.005f, 0.0f, 0.0f, "X: %.1f");
		ImGui::DragFloat("Light Direction Y", &lighDir.y, 0.005f, 0.0f, 0.0f, "Y: %.1f");
		ImGui::DragFloat("Light Direction Z", &lighDir.z, 0.005f, 0.0f, 0.0f, "Z: %.1f");
		m_uiManager.EndWindow();

		m_uiManager.EndFrame();
		m_uiManager.Render();

		m_editor.Update(clock.GetDeltaTime());
		m_editor.PostUpdate();

		clock.Update();
	}
}

bool Example::Application::IsRunning()
{
	return isRunning && m_context.m_window->IsActive();
}
