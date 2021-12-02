#include "pch.h"

#include "Application.h"

#include <AmberEngine/Tools/Time/Clock.h>
/*#include <AmberEngine/ImGUI/imgui.h>
#include <AmberEngine/Resources/Primitives/Cube.h>
#include <AmberEngine/Buffers/VertexBuffer.h>
#include <AmberEngine/Buffers/VertexArray.h>*/

Example::Application::Application(const AmberEngine::Settings::DeviceSettings & p_deviceSettings,
	const AmberEngine::Settings::WindowSettings & p_windowSettings,
	const AmberEngine::Settings::DriverSettings & p_driverSettings) :
	m_context(p_deviceSettings, p_windowSettings, p_driverSettings),
	m_editor(m_context)
{
	isRunning = true;
}

void Example::Application::Setup()
{
	m_context.resourcesManager.LoadModel("Helmet", "res/Mesh/DamagedHelmet/glTF/DamagedHelmet.gltf");

	AmberEngine::Resources::Shader& lightingShader = m_context.resourcesManager.LoadShader("StandardLighting", "res/shaders/StandardLighting.glsl");
	
	//m_context.resourcesManager.LoadTexture("diffuse", "crystal.jpg", AmberEngine::Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::DIFFUSE,true, true);
	//m_context.resourcesManager.LoadTexture("specular", "crystal_spec.jpg", AmberEngine::Settings::ETextureFilteringMode::NEAREST_MIPMAP_LINEAR, AmberEngine::Settings::ETextureFilteringMode::NEAREST, AmberEngine::Settings::ETextureType::DIFFUSE, true, true);
	m_context.resourcesManager.GetModel("Helmet").SetShader(lightingShader);
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

		glm::vec3 cameraPosition = m_editor.m_sceneView.GetCameraController().GetPosition();
		glm::mat4 projectionMatrix = m_editor.m_sceneView.GetCameraController().GetCamera().GetProjectionMatrix();
		glm::mat4 viewMatrix = m_editor.m_sceneView.GetCameraController().GetCamera().GetViewMatrix();
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		auto& shader = m_context.resourcesManager.GetShader("StandardLighting");
		
		shader.Bind();
		shader.SetUniformMat4("projection", projectionMatrix);
		shader.SetUniformMat4("view", viewMatrix);
		shader.SetUniformMat4("model", modelMatrix);
		shader.SetUniformVec3("viewPos", cameraPosition);
		shader.SetUniformVec3("light.direction", lighDir);

		m_context.renderer->Draw(m_context.resourcesManager.GetModel("Helmet"));

		shader.Unbind();

		m_editor.Update(clock.GetDeltaTime());
		m_editor.RenderScene();
		m_editor.PostUpdate();

		clock.Update();
	}
}

bool Example::Application::IsRunning() const
{
	return isRunning && m_context.window->IsActive();
}
