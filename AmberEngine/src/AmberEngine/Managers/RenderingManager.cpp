#include "AmberEngine/Managers/RenderingManager.h"

AmberEngine::Managers::RenderingManager::RenderingManager() : isWireFrame(false), isCameraFree(true)
{
	m_windowManager = std::make_unique<WindowManager>();
	m_camera = std::make_unique<LowRenderer::Camera>(m_windowManager->GetDevice(), glm::vec3(0.0f, 0.0f, 3.0f));
	m_resourcesManager = std::make_unique<ResourcesManager>();
	m_uiManager = std::make_unique<UIManager>(m_windowManager->GetDevice());
	m_inputManager = std::make_unique<InputManager>();
	isRunning = true;
}

void AmberEngine::Managers::RenderingManager::SetWindow(uint16_t p_width, uint16_t p_height)
{
	//TODO
}

void AmberEngine::Managers::RenderingManager::SetCamera(const glm::vec3& p_position)
{
	//TODO
}

void AmberEngine::Managers::RenderingManager::Clear()
{
	//GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
	//GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	m_uiManager->PreUpdate();
}

void AmberEngine::Managers::RenderingManager::Update()
{
	m_windowManager->Update();
	UpdateDeltaTime();
	UpdateRenderMode();
	m_camera->Update(m_deltaTime);
	m_uiManager->Update();
	m_inputManager->Update();
}

void AmberEngine::Managers::RenderingManager::SwapBuffers()
{
	m_uiManager->PostUpdate();
	m_windowManager->GetDevice().SwapBuffers();
	m_windowManager->GetDevice().PollEvents();
}

bool AmberEngine::Managers::RenderingManager::IsRunning()
{
	return isRunning && m_windowManager->IsOpen();
}

void AmberEngine::Managers::RenderingManager::UpdateRenderMode()
{
	if (m_inputManager->IsKeyEventOccured(0x10))
	{
		isWireFrame = !isWireFrame;
	}
	if (m_inputManager->IsKeyEventOccured(0x12))
	{
		isCameraFree = !isCameraFree;
	}

	if (isWireFrame)
		PolygonModeLine();
	else
		PolygonModeFill();

	if (isCameraFree)
	{
		m_camera->Unlock();
		m_windowManager->GetDevice().LockCursor();
	}
	else
	{
		m_windowManager->GetDevice().FreeCursor();
		m_camera->Lock();
	}
}

void AmberEngine::Managers::RenderingManager::UpdateDeltaTime()
{
	GLdouble currentTime = glfwGetTime();
	m_deltaTime = currentTime - m_lastTime;
	m_lastTime = currentTime;
}

void AmberEngine::Managers::RenderingManager::PolygonModeLine()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void AmberEngine::Managers::RenderingManager::PolygonModeFill()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

const std::unique_ptr<AmberEngine::Managers::WindowManager>& AmberEngine::Managers::RenderingManager::GetWindowManager() const
{
	return m_windowManager;
}

const std::unique_ptr<AmberEngine::Managers::ResourcesManager>& AmberEngine::Managers::RenderingManager::
GetResourcesManager() const
{
	return m_resourcesManager;
}

glm::mat4 AmberEngine::Managers::RenderingManager::CalculateProjectionMatrix() const
{
	return m_camera->GetProjectionMatrix();
}

glm::mat4 AmberEngine::Managers::RenderingManager::CalculateViewMatrix() const
{
	return m_camera->GetViewMatrix();
}

glm::mat4 AmberEngine::Managers::RenderingManager::GetModelMatrix() const
{
	return glm::mat4(1.0f);
}

const std::unique_ptr<AmberEngine::LowRenderer::Camera>& AmberEngine::Managers::RenderingManager::GetCamera() const
{
	return m_camera;
}