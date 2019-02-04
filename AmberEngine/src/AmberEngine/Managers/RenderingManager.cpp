#include "AmberEngine/Managers/RenderingManager.h"
#include "AmberEngine/Managers/InputManager.h"

AmberEngine::Managers::RenderingManager::RenderingManager(const Settings::RenderingSettings& p_settings)
	: m_windowManager(p_settings.deviceSettings, p_settings.driverSettings), m_cameraController(m_windowManager.GetDevice(), glm::vec3(0.0f, 0.0f, 3.0f)) ,isWireFrame(false), isCameraFree(true)
{
	isRunning = true;
}

void AmberEngine::Managers::RenderingManager::SetWindow(uint16_t p_width, uint16_t p_height)
{
	//TODO
}

void AmberEngine::Managers::RenderingManager::SetCameraPosition(const glm::vec3& p_position)
{
	m_cameraController.GetCamera().SetPosition(p_position);
}

void AmberEngine::Managers::RenderingManager::Clear(float p_red, float p_green, float p_blue, float p_alpha)
{
	glClearColor(p_red, p_green, p_blue, p_alpha);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AmberEngine::Managers::RenderingManager::Update()
{
	UpdateDeltaTime();
	UpdateRenderMode();
	m_cameraController.Update(m_deltaTime);
	UpdateInput();
	InputManager::Update();
}

void AmberEngine::Managers::RenderingManager::SwapBuffers()
{
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
	if (InputManager::GetKeyDown(InputManager::Key::KEY_LEFT_SHIFT))
		ToggleWireFrame();
	
	if (InputManager::GetKeyDown(InputManager::Key::KEY_LEFT_ALT))
		ToggleCamera();
	
	if (InputManager::GetKeyDown(InputManager::Key::KEY_ESCAPE))
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
	m_cameraController.Unlock();
	m_windowManager.GetDevice().LockCursor();
}

void AmberEngine::Managers::RenderingManager::LockCamera()
{
	m_cameraController.Lock();
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

AmberEngine::Managers::ResourcesManager& AmberEngine::Managers::RenderingManager::GetResourcesManager()
{
	return m_resourcesManager;
}

glm::mat4 AmberEngine::Managers::RenderingManager::CalculateProjectionMatrix()
{
	return m_cameraController.GetProjectionMatrix();
}

glm::mat4 AmberEngine::Managers::RenderingManager::CalculateViewMatrix()
{
	return m_cameraController.GetCamera().GetViewMatrix();
}

glm::mat4 AmberEngine::Managers::RenderingManager::GetUnitModelMatrix()
{
	return glm::mat4(1.0f);
}

AmberEngine::LowRenderer::CameraController& AmberEngine::Managers::RenderingManager::GetCameraController()
{
	return m_cameraController;
}
