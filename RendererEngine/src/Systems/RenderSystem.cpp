#include "pch.h"

#include "Systems/RenderSystem.h"
#include "Resources/Mesh/Cube.h"
#include "Resources/Texture.h"

RenderEngine::Systems::RenderSystem::RenderSystem()
{
	InitOpenGL();
}

RenderEngine::Systems::RenderSystem::~RenderSystem()
{
	m_context.Close();
}

void RenderEngine::Systems::RenderSystem::InitOpenGL()
{
	GLCall(const GLenum error = glewInit());
	if (error != GLEW_OK)
	{
		std::cout << "Error Init GLEW: " << glewGetErrorString(error) << std::endl;
	}

	std::cout << "Using GLEW: " << glewGetString(GLEW_VERSION) << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_VENDOR) << std::endl;
	std::cout << glGetString(GL_RENDERER) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	GLCall(glEnable(GL_DEBUG_OUTPUT));

	GLCall(glEnable(GL_DEPTH_TEST));

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/
}

void RenderEngine::Systems::RenderSystem::Run()
{
	PrimitiveMesh::Cube cube;
	cube.InitTexturedCube();

	m_camera = std::make_unique<LowRenderer::Camera>(m_context, glm::vec3(0.0f, 0.0f, 3.0f));

	Resources::Shader lightingShader("res/shaders/lighting_maps.vs", "res/shaders/lighting_maps.fs");
	Resources::Shader lampShader("res/shaders/lamp.vs", "res/shaders/lamp.fs");

	Resources::Texture diffuseMap("res/textures/crystal.jpg");;
	Resources::Texture specularMap("res/textures/crystal_spec.jpg");;

	lightingShader.Bind();
	lightingShader.SetUniform1i("material.diffuse", 0);
	lightingShader.SetUniform1i("material.specular", 1);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(m_context.GetContextWindow(), true);
	ImGui::StyleColorsDark();

	while (!glfwWindowShouldClose(m_context.GetContextWindow()))
	{
		GLdouble currentTime = glfwGetTime();
		m_deltaTime = currentTime - m_lastTime;
		m_lastTime = currentTime;

		PreUpdate();

		ImGui_ImplGlfwGL3_NewFrame();

		Update();

		{
			lightingShader.Bind();

			lightingShader.SetUniformVec3("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
			lightingShader.SetUniformVec3("viewPos", m_camera->m_position);

			lightingShader.SetUniformVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			lightingShader.SetUniformVec3("light.diffuse", glm::vec3(0.1f, 0.1f, 0.1f));
			lightingShader.SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

			lightingShader.SetUniform1f("material.shininess", 20.0f);
		}
		
		glm::mat4 projection = glm::perspective(glm::radians(m_camera->GetCameraZoom()), static_cast<float>(m_context.GetWidthWindow()) / static_cast<float>(m_context.GetHeightWindow()), 0.1f, 100.0f);
		glm::mat4 view = m_camera->GetViewMatrix();
		lightingShader.SetUniformMat4("projection", projection);
		lightingShader.SetUniformMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.SetUniformMat4("model", model);

		diffuseMap.Bind();
		specularMap.Bind(1);

		cube.Draw();
		++m_drawCallCount;

		lampShader.Bind();
		lampShader.SetUniformMat4("projection", projection);
		lampShader.SetUniformMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.2f, 1.0f, 2.0f));
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		lampShader.SetUniformMat4("model", model);

		cube.Draw();
		++m_drawCallCount;

		{
			ImGui::Text("Scene");
			ImGui::Text("DrawCalls/frame: %d", m_drawCallCount);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Bullet();
			ImGui::Text("Camera Transform \nPosition X: %f Y: %f Z: %f", m_camera->m_position.x, m_camera->m_position.y, m_camera->m_position.z);
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		PostUpdate();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
}

void RenderEngine::Systems::RenderSystem::PreUpdate()
{
	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	m_drawCallCount = 0;
}

void RenderEngine::Systems::RenderSystem::Update()
{
	m_context.Update();
	m_camera->Update(m_deltaTime);

	if (glfwGetKey(m_context.GetContextWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderEngine::Systems::RenderSystem::PostUpdate()
{
	glfwSwapBuffers(m_context.GetContextWindow());
	glfwPollEvents();
}