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
	GLfloat vertices2[] = { 1, 1, 1,  -1, 1, 1,  -1,-1, 1,   1,-1, 1,   // v0,v1,v2,v3
						1,-1,-1,   1, 1,-1,  -1, 1,-1,  -1,-1,-1 }; // v4,v7,v6,v5 (back)

	GLfloat normals2[] = { 0, 0, 1,   0, 0, 1,   0, 0, 1,   0, 0, 1,   // v0,v1,v2,v3 (front)
						1, 0, 0,   1, 0, 0,   1, 0, 0,   1, 0, 0,   // v0,v3,v4,v5 (right)
						0, 1, 0,   0, 1, 0,   0, 1, 0,   0, 1, 0,   // v0,v5,v6,v1 (top)
					   -1, 0, 0,  -1, 0, 0,  -1, 0, 0,  -1, 0, 0,   // v1,v6,v7,v2 (left)
						0,-1, 0,   0,-1, 0,   0,-1, 0,   0,-1, 0,   // v7,v4,v3,v2 (bottom)
						0, 0,-1,   0, 0,-1,   0, 0,-1,   0, 0,-1 }; // v4,v7,v6,v5 (back)

	GLubyte indices[] = { 0, 1, 2,   2, 3, 0,      // front
					   0, 3, 4,   4, 5, 0,      // right
					   0, 5, 6,   6, 1, 0,      // top
					   1, 6, 7,   7, 2, 1,      // left
					   4, 3, 2,   2, 7, 4,      // bottom
					   7, 6, 5,   5, 4, 7 };    // back

	//test
	GLfloat cube_vertices[] = {
		// front
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
	};

	GLushort cube_elements[] = {
    // front
    0, 1, 2,
    2, 3, 0,
    // top
    1, 5, 6,
    6, 2, 1,
    // back
    7, 6, 5,
    5, 4, 7,
    // bottom
    4, 0, 3,
    3, 7, 4,
    // left
    4, 5, 1,
    1, 0, 4,
    // right
    3, 2, 6,
    6, 7, 3,
  };

	unsigned int VBO, vbonormal, cubeVAO, EBO;
	glGenVertexArrays(1, &cubeVAO);

	glBindVertexArray(cubeVAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,3 * sizeof(float), (void*)0);

	//glGenBuffers(1, &vbonormal);
	//glBindBuffer(GL_ARRAY_BUFFER, vbonormal);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(normals2), normals2, GL_STATIC_DRAW);
	//
	//glEnableVertexAttribArray(1);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
		//shader.Unbind();
		
		m_context.m_resourcesManager.GetTexture("diffuse").Bind();
		m_context.m_resourcesManager.GetTexture("specular").Bind(1);

		//m_context.m_renderer->DrawModelWithShader(m_context.m_resourcesManager.GetModel("Suit"), shader);
		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);

		
		

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
