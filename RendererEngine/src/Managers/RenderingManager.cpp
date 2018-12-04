#include "pch.h"

#include "Managers/RenderingManager.h"

RenderEngine::Managers::RenderingManager::RenderingManager() : isWireFrame(false)
{
	m_windowManager = std::make_unique<WindowManager>();
	m_camera = std::make_unique<LowRenderer::Camera>(m_windowManager->GetDevice(), glm::vec3(0.0f, 0.0f, 3.0f));
	m_resourcesManager = std::make_unique<ResourcesManager>();
	m_uiManager = std::make_unique<UIManager>(m_windowManager->GetDevice());
	isRunning = true;
}

void RenderEngine::Managers::RenderingManager::SetWindow(uint16_t p_width, uint16_t p_height)
{
	//TODO
}

void RenderEngine::Managers::RenderingManager::SetCamera(const glm::vec3& p_position)
{
	//TODO
}

void RenderEngine::Managers::RenderingManager::Clear()
{
	//GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	//GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	m_uiManager->PreUpdate();
}

void RenderEngine::Managers::RenderingManager::Update()
{
	m_windowManager->Update();
	UpdateDeltaTime();
	UpdateRenderMode();
	m_camera->Update(m_deltaTime);
	m_uiManager->Update();
}

void RenderEngine::Managers::RenderingManager::SwapBuffers()
{
	m_uiManager->PostUpdate();
	m_windowManager->GetDevice().SwapBuffers();
	m_windowManager->GetDevice().PollEvents();
}

bool RenderEngine::Managers::RenderingManager::IsRunning()
{
	return isRunning && m_windowManager->IsOpen();
}

void RenderEngine::Managers::RenderingManager::UpdateRenderMode()
{
	static int oldState = Core::Device::GetPressState();
	const int newState = glfwGetKey(m_windowManager->GetDevice().GetContextWindow(), GLFW_KEY_LEFT_SHIFT);

	if (newState == Core::Device::GetPressState() && oldState == Core::Device::GetReleaseState()) 
	{
		isWireFrame = !isWireFrame;
	}
	oldState = newState;

	if (isWireFrame)
		PolygonModeLine();
	else
		PolygonModeFill();
}

void RenderEngine::Managers::RenderingManager::UpdateDeltaTime()
{
	GLdouble currentTime = glfwGetTime();
	m_deltaTime = currentTime - m_lastTime;
	m_lastTime = currentTime;
}

void RenderEngine::Managers::RenderingManager::PolygonModeLine()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void RenderEngine::Managers::RenderingManager::PolygonModeFill()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

const std::unique_ptr<RenderEngine::Managers::WindowManager>& RenderEngine::Managers::RenderingManager::GetWindowManager() const
{
	return m_windowManager;
}

const std::unique_ptr<RenderEngine::Managers::ResourcesManager>& RenderEngine::Managers::RenderingManager::
GetResourcesManager() const
{
	return m_resourcesManager;
}

glm::mat4 RenderEngine::Managers::RenderingManager::CalculateProjectionMatrix() const
{
	return m_camera->GetProjectionMatrix();
}

glm::mat4 RenderEngine::Managers::RenderingManager::CalculateViewMatrix() const
{
	return m_camera->GetViewMatrix();
}

glm::mat4 RenderEngine::Managers::RenderingManager::GetModelMatrix() const
{
	return glm::mat4(1.0f);
}

const std::unique_ptr<RenderEngine::LowRenderer::Camera>& RenderEngine::Managers::RenderingManager::GetCamera() const
{
	return m_camera;
}