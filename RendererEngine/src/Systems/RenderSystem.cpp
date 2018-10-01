#include "pch.h"

#include "Systems/RenderSystem.h"
#include "Resources/Mesh/Cube.h"
#include "Resources/Texture.h"

RenderEngine::Systems::RenderSystem::RenderSystem()
{
	m_context = std::make_unique<Core::Context>();
	InitOpenGL();
}

RenderEngine::Systems::RenderSystem::~RenderSystem()
{
	m_context->Close();
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
	cube.BindTexturedCube();

	m_camera = std::make_unique<LowRenderer::Camera>(*m_context, glm::vec3(0.0f, 0.0f, 3.0f));
	
	Resources::Shader lightingShader("res/shaders/lighting_maps.vs", "res/shaders/lighting_maps.fs");
	Resources::Shader lampShader = m_resourcesManager.LoadShaderFiles("lamp", "lamp.vs", "lamp.fs");

	Resources::Texture diffuseMap("res/textures/crystal.jpg");;
	Resources::Texture specularMap("res/textures/crystal_spec.jpg");;

	lightingShader.Bind();
	lightingShader.SetUniform1i("material.diffuse", 0);
	lightingShader.SetUniform1i("material.specular", 1);

	m_uiSystem = std::make_unique<UISystem>(*m_context);

	while (!glfwWindowShouldClose(m_context->GetContextWindow()))
	{
		GLdouble currentTime = glfwGetTime();
		m_deltaTime = currentTime - m_lastTime;
		m_lastTime = currentTime;

		PreUpdate();
		Update();

		{
			lightingShader.Bind();

			lightingShader.SetUniformVec3("light.position", glm::vec3(1.2f, 1.0f, 2.0f));
			lightingShader.SetUniformVec3("viewPos", m_camera->m_position);

			lightingShader.SetUniformVec3("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
			lightingShader.SetUniformVec3("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
			lightingShader.SetUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

			lightingShader.SetUniform1f("material.shininess", 20.0f);
		
		
			glm::mat4 projection = glm::perspective(glm::radians(m_camera->GetCameraZoom()), static_cast<float>(m_context->GetWidthWindow()) / static_cast<float>(m_context->GetHeightWindow()), 0.1f, 100.0f);
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
		}

		PostUpdate();
	}

	m_uiSystem->Close();
}

void RenderEngine::Systems::RenderSystem::PreUpdate()
{
	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	m_drawCallCount = 0;

	m_uiSystem->PreUpdate();
}

void RenderEngine::Systems::RenderSystem::Update()
{
	m_context->Update();
	m_camera->Update(m_deltaTime);
	m_uiSystem->Update(m_drawCallCount, *m_camera);

	if (glfwGetKey(m_context->GetContextWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderEngine::Systems::RenderSystem::PostUpdate()
{
	m_uiSystem->PostUpdate();

	glfwSwapBuffers(m_context->GetContextWindow());
	glfwPollEvents();
}