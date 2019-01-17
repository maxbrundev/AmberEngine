#include <Windows.h>

#include "AmberEngine/Managers/RenderingManager.h"

AmberEngine::Managers::RenderingManager::RenderingManager() 
	: m_uiManager(m_windowManager.GetDevice()), m_camera(m_windowManager.GetDevice(), glm::vec3(0.0f, 0.0f, 3.0f)) ,isWireFrame(false), isCameraFree(true)
{
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
	m_uiManager.PreUpdate();
}

void AmberEngine::Managers::RenderingManager::Update()
{
	UpdateDeltaTime();
	UpdateRenderMode();
	m_camera.Update(m_deltaTime);
	m_uiManager.Update();
	UpdateInput();
}

void AmberEngine::Managers::RenderingManager::SwapBuffers()
{
	m_uiManager.PostUpdate();
	m_windowManager.GetDevice().SwapBuffers();
	m_windowManager.GetDevice().PollEvents();
}

bool AmberEngine::Managers::RenderingManager::IsRunning()
{
	return isRunning && m_windowManager.IsOpen();
}

void AmberEngine::Managers::RenderingManager::UpdateRenderMode()
{
	if (isWireFrame)
		PolygonModeLine();
	else
		PolygonModeFill();

	if (isCameraFree)
		FreeCamera();
	else
		LockCamera();
	
}

void AmberEngine::Managers::RenderingManager::UpdateDeltaTime()
{
	GLdouble currentTime = glfwGetTime();
	m_deltaTime = currentTime - m_lastTime;
	m_lastTime = currentTime;
}

void AmberEngine::Managers::RenderingManager::UpdateInput()
{
	m_inputManager.Update();

	if (m_inputManager.IsKeyEventOccured(VK_SHIFT))
	{
		ToggleWireFrame();
	}
	if (m_inputManager.IsKeyEventOccured(VK_LMENU))
	{
		ToggleCamera();
	}

	if (m_inputManager.IsKeyPressed(VK_ESCAPE))
		m_windowManager.GetDevice().Close();
}

void AmberEngine::Managers::RenderingManager::PolygonModeLine()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void AmberEngine::Managers::RenderingManager::PolygonModeFill()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void AmberEngine::Managers::RenderingManager::ToggleWireFrame()
{
	isWireFrame = !isWireFrame;
}

void AmberEngine::Managers::RenderingManager::FreeCamera()
{
	m_camera.Unlock();
	m_windowManager.GetDevice().LockCursor();
}

void AmberEngine::Managers::RenderingManager::LockCamera()
{
	m_camera.Lock();
	m_windowManager.GetDevice().FreeCursor();
}

void AmberEngine::Managers::RenderingManager::ToggleCamera()
{
	isCameraFree = !isCameraFree;
}

AmberEngine::Managers::WindowManager& AmberEngine::Managers::RenderingManager::GetWindowManager()
{
	return m_windowManager;
}

AmberEngine::Managers::ResourcesManager& AmberEngine::Managers::RenderingManager::
GetResourcesManager()
{
	return m_resourcesManager;
}

AmberEngine::Managers::InputManager& AmberEngine::Managers::RenderingManager::
GetInputManager()
{
	return m_inputManager;
}

glm::mat4 AmberEngine::Managers::RenderingManager::CalculateProjectionMatrix() const
{
	return m_camera.GetProjectionMatrix();
}

glm::mat4 AmberEngine::Managers::RenderingManager::CalculateViewMatrix() const
{
	return m_camera.GetViewMatrix();
}

glm::mat4 AmberEngine::Managers::RenderingManager::GetModelMatrix() const
{
	return glm::mat4(1.0f);
}

AmberEngine::LowRenderer::Camera& AmberEngine::Managers::RenderingManager::GetCamera()
{
	return m_camera;
}