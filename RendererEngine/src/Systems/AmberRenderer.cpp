#include "pch.h"

#include "Systems/AmberRenderer.h"
#include "Resources/Mesh/Cube.h"

RenderEngine::Systems::AmberRenderer::AmberRenderer()
{
	m_renderingManager = std::make_unique<Managers::RenderingManager>();
	m_resourcesManager = std::make_unique<Managers::ResourcesManager>();

	m_renderingManager->SetCamera(glm::vec3(0.0f, 0.0f, 3.0f));
}

void RenderEngine::Systems::AmberRenderer::Setup()
{
	PrimitiveMesh::Cube::Setup();
	auto& vertices = PrimitiveMesh::Cube::GetVertices();

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PrimitiveMesh::Vertex), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveMesh::Vertex), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveMesh::Vertex), reinterpret_cast<void*>(offsetof(PrimitiveMesh::Vertex, normals)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveMesh::Vertex), reinterpret_cast<void*>(offsetof(PrimitiveMesh::Vertex, textureCoord)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Resources::Shader& lightingShader = m_resourcesManager->LoadShaderFiles("LightingMaps", "lighting_maps.vs", "lighting_maps.fs");
	Resources::Shader& lampShader = m_resourcesManager->LoadShaderFiles("lamp", "lamp.vs", "lamp.fs");

	Resources::Texture& diffuseMap = m_resourcesManager->LoadTexture("diffuse", "abs.jpg");
	Resources::Texture& specularMap = m_resourcesManager->LoadTexture("specular", "abs_spec.jpg");
	Resources::Texture& emissionMap = m_resourcesManager->LoadTexture("emissive", "matrix_emissive.jpg");

	lightingShader.Bind();
	lightingShader.SetUniform1i("material.diffuse", 0);
	lightingShader.SetUniform1i("material.specular", 1);
	lightingShader.SetUniform1i("material.emission", 2);
	lightingShader.Unbind();

	m_lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
}

void RenderEngine::Systems::AmberRenderer::Run()
{
	while (m_renderingManager->IsRunning())
	{
		m_renderingManager->PreUpdate();
		m_renderingManager->Update();

		//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

		glm::mat4 projectionMatrix = m_renderingManager->CalculateProjectionMatrix();
		glm::mat4 viewMatrix = m_renderingManager->CalculateViewMatrix();
		glm::mat4 modelMatrix = m_renderingManager->CalculateModelMatrix();

		modelMatrix = glm::rotate(modelMatrix, static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));

		m_resourcesManager->GetShader("LightingMaps").Bind();
		m_resourcesManager->GetShader("LightingMaps").SetUniformMat4("projection", projectionMatrix);
		m_resourcesManager->GetShader("LightingMaps").SetUniformMat4("view", viewMatrix);
		m_resourcesManager->GetShader("LightingMaps").SetUniformMat4("model", modelMatrix);
		m_resourcesManager->GetShader("LightingMaps").SetUniformVec3("viewPos", m_renderingManager->GetCamera()->GetPosition());

		m_resourcesManager->GetShader("LightingMaps").SetUniformVec3("light.position", m_lightPos);
		m_resourcesManager->GetShader("LightingMaps").SetUniformVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		m_resourcesManager->GetShader("LightingMaps").SetUniformVec3("light.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		m_resourcesManager->GetShader("LightingMaps").SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		m_resourcesManager->GetShader("LightingMaps").SetUniform1f("material.shininess", 5.0f);
		m_resourcesManager->GetShader("LightingMaps").SetUniform1f("time", glfwGetTime());

		m_resourcesManager->GetTexture("diffuse").Bind();
		m_resourcesManager->GetTexture("specular").Bind(1);
		m_resourcesManager->GetTexture("emissive").Bind(2);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, m_lightPos);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f)); // a smaller cube

		m_resourcesManager->GetShader("lamp").Bind();
		m_resourcesManager->GetShader("lamp").SetUniformMat4("projection", projectionMatrix);
		m_resourcesManager->GetShader("lamp").SetUniformMat4("view", viewMatrix);
		m_resourcesManager->GetShader("lamp").SetUniformMat4("model", modelMatrix);

		glBindVertexArray(m_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		ImGui::Text("Specular Texture");
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(m_resourcesManager->GetTexture("specular").GetTextureId())), ImVec2(256, 256));

		m_renderingManager->PostUpdate();
	}
}