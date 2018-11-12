#include "pch.h"

#include "Systems/RenderSystem.h"
#include "Resources/Mesh/Cube.h"

RenderEngine::Systems::RenderSystem::RenderSystem()
{
	m_renderingManager = std::make_unique<Managers::RenderingManager>();
	m_resourcesManager = std::make_unique<Managers::ResourcesManager>();
}

void RenderEngine::Systems::RenderSystem::Run()
{
	PrimitiveMesh::Cube::Setup();
	auto& vertices = PrimitiveMesh::Cube::GetVertices();

	GLuint vao;
	GLuint vbo;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(PrimitiveMesh::Vertex), vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveMesh::Vertex), nullptr);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(PrimitiveMesh::Vertex), reinterpret_cast<void*>(offsetof(PrimitiveMesh::Vertex, normals)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(PrimitiveMesh::Vertex), reinterpret_cast<void*>(offsetof(PrimitiveMesh::Vertex, textureCoord)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Resources::Shader lightingShader("res/shaders/lighting_maps.vs", "res/shaders/lighting_maps.fs");
	Resources::Shader lampShader = m_resourcesManager->LoadShaderFiles("lamp", "lamp.vs", "lamp.fs");

	Resources::Texture diffuseMap("res/textures/abs.jpg");
	Resources::Texture specularMap("res/textures/abs_spec.jpg");
	Resources::Texture emissionMap("res/textures/matrix_emissive.jpg");

	lightingShader.Bind();
	lightingShader.SetUniform1i("material.diffuse", 0);
	lightingShader.SetUniform1i("material.specular", 1);
	lightingShader.SetUniform1i("material.emission", 2);
	lightingShader.Unbind();

	glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

	while (m_renderingManager->IsRunning())
	{
		m_renderingManager->PreUpdate();
		m_renderingManager->Update();

		//lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		//lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

		glm::mat4 projectionMatrix = m_renderingManager->CalculateProjectionMatrix();
		glm::mat4 viewMatrix = m_renderingManager->CalculateViewMatrix();
		glm::mat4 modelMatrix = glm::mat4(1.0f);

		modelMatrix = glm::rotate(modelMatrix, static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));

		lightingShader.Bind();
		lightingShader.SetUniformMat4("projection", projectionMatrix);
		lightingShader.SetUniformMat4("view", viewMatrix);
		lightingShader.SetUniformMat4("model", modelMatrix);
		lightingShader.SetUniformVec3("viewPos", m_renderingManager->GetCamera()->GetPosition());

		lightingShader.SetUniformVec3("light.position", lightPos);
		lightingShader.SetUniformVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
		lightingShader.SetUniformVec3("light.diffuse", glm::vec3(0.2f, 0.2f, 0.2f));
		lightingShader.SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		lightingShader.SetUniform1f("material.shininess", 5.0f);
		lightingShader.SetUniform1f("time", glfwGetTime());

		diffuseMap.Bind();
		specularMap.Bind(1);
		emissionMap.Bind(2);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, lightPos);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f)); // a smaller cube

		lampShader.Bind();
		lampShader.SetUniformMat4("projection", projectionMatrix);
		lampShader.SetUniformMat4("view", viewMatrix);
		lampShader.SetUniformMat4("model", modelMatrix);

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		ImGui::Text("Specular Texture");
		ImGui::Image(reinterpret_cast<void*>(static_cast<intptr_t>(specularMap.GetTextureId())), ImVec2(256, 256));

		m_renderingManager->PostUpdate();
	}
}