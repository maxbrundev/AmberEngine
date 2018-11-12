#include "pch.h"

#include "Managers/RenderingManager.h"

RenderEngine::Managers::RenderingManager::RenderingManager()
{
	m_windowManager = std::make_unique<WindowManager>();
	m_camera = std::make_unique<LowRenderer::Camera>(m_windowManager->GetDevice(), glm::vec3(0.0f, 0.0f, 3.0f));
	m_uiManager = std::make_unique<UIManager>(m_windowManager->GetDevice());

	isRunning = true;
}

RenderEngine::Managers::RenderingManager::~RenderingManager()
{
	m_uiManager->Close();
}

void RenderEngine::Managers::RenderingManager::PreUpdate()
{
	//GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	//GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

	m_uiManager->PreUpdate();
}

void RenderEngine::Managers::RenderingManager::Update()
{
	GLdouble currentTime = glfwGetTime();
	m_deltaTime = currentTime - m_lastTime;
	m_lastTime = currentTime;

	m_windowManager->Update();
	UpdateRenderMode();
	m_camera->Update(m_deltaTime);
	m_uiManager->Update(*m_camera);
}

void RenderEngine::Managers::RenderingManager::PostUpdate()
{
	m_uiManager->PostUpdate();

	glfwSwapBuffers(m_windowManager->GetDevice().GetContextWindow());
	glfwPollEvents();
}

bool RenderEngine::Managers::RenderingManager::IsRunning()
{
	return isRunning && m_windowManager->IsOpen();
}

void RenderEngine::Managers::RenderingManager::UpdateRenderMode()
{
	if (glfwGetKey(m_windowManager->GetDevice().GetContextWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

const std::unique_ptr<RenderEngine::Managers::WindowManager>& RenderEngine::Managers::RenderingManager::GetWindowManager() const
{
	return m_windowManager;
}

glm::mat4 RenderEngine::Managers::RenderingManager::CalculateProjectionMatrix() const
{
	return m_camera->GetProjectionMatrix();
}

glm::mat4 RenderEngine::Managers::RenderingManager::CalculateViewMatrix() const
{
	return m_camera->GetViewMatrix();
}

const std::unique_ptr<RenderEngine::LowRenderer::Camera>& RenderEngine::Managers::RenderingManager::GetCamera() const
{
	return m_camera;
}
