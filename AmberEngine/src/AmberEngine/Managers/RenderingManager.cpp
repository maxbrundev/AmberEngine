#include "Amberpch.h"

#include "AmberEngine/Managers/RenderingManager.h"

AmberEngine::Managers::RenderingManager::RenderingManager(const Settings::DeviceSettings& p_deviceSettings, const Settings::WindowSettings& p_windowSettings, const Settings::DriverSettings& p_driverSettings)
	: isWireFrame(false), isCameraFree(true)
{
	m_device = std::make_unique<Context::Device>(p_deviceSettings);
	m_window = std::make_unique<Context::Window>(*m_device, p_windowSettings);
	m_driver = std::make_unique<Context::Driver>(p_driverSettings);

	m_inputManager = std::make_unique<Inputs::InputManager>(*m_window);

	m_cameraController = std::make_shared<LowRenderer::CameraController>(*m_window, *m_inputManager, glm::vec3(0.0f, 0.0f, 3.0f));
	
	isRunning = true;
}

void AmberEngine::Managers::RenderingManager::SetCameraPosition(const glm::vec3& p_position)
{
	m_cameraController->GetCamera().SetPosition(p_position);
}

void AmberEngine::Managers::RenderingManager::SetClearColor(float p_red, float p_green, float p_blue, float p_alpha)
{
	glClearColor(p_red, p_green, p_blue, p_alpha);
}

void AmberEngine::Managers::RenderingManager::Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
{
	glClear
	(
		(p_colorBuffer ? GL_COLOR_BUFFER_BIT : 0) |
		(p_depthBuffer ? GL_DEPTH_BUFFER_BIT : 0) |
		(p_stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
	);
}

void AmberEngine::Managers::RenderingManager::Clear(LowRenderer::Camera& p_camera, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
{
	/* Backup the previous OpenGL clear color */
	GLfloat previousClearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, previousClearColor);

	/* Clear the screen using the camera clear color */
	const glm::vec3& cameraClearColor = p_camera.GetClearColor();
	SetClearColor(cameraClearColor.x, cameraClearColor.y, cameraClearColor.z, 1.0f);
	Clear(p_colorBuffer, p_depthBuffer, p_stencilBuffer);

	/* Reset the OpenGL clear color to the previous clear color (Backuped one) */
	SetClearColor(previousClearColor[0], previousClearColor[1], previousClearColor[2], previousClearColor[3]);
}

void AmberEngine::Managers::RenderingManager::Update()
{
	UpdateDeltaTime();
	UpdateRenderMode();
	m_cameraController->Update(m_deltaTime);
	UpdateInput();
	m_inputManager->clearEvents();
}

void AmberEngine::Managers::RenderingManager::SwapBuffers()
{
	m_window->SwapBuffers();
	m_device->PollEvents();
}

bool AmberEngine::Managers::RenderingManager::IsRunning()
{
	return isRunning && m_window->IsActive();
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
	float currentTime = glfwGetTime();
	m_deltaTime = currentTime - m_lastTime;
	m_lastTime = currentTime;
}

void AmberEngine::Managers::RenderingManager::UpdateInput()
{
	if (m_inputManager->IsKeyPressed(Inputs::EKey::KEY_LEFT_SHIFT))
		ToggleWireFrame();
	
	if (m_inputManager->IsKeyPressed(Inputs::EKey::KEY_LEFT_ALT))
		ToggleCamera();
	
	if (m_inputManager->IsKeyPressed(Inputs::EKey::KEY_ESCAPE))
		m_window->SetShouldClose(true);
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
	m_cameraController->Unlock();
	m_window->SetCursorModeLock();
}

void AmberEngine::Managers::RenderingManager::LockCamera()
{
	m_cameraController->Lock();
	m_window->SetCursorModeFree();
}

void AmberEngine::Managers::RenderingManager::ToggleCamera()
{
	isCameraFree = !isCameraFree;
}

AmberEngine::Context::Window& AmberEngine::Managers::RenderingManager::GetWindow()
{
	return *m_window;
}

AmberEngine::Managers::ResourcesManager& AmberEngine::Managers::RenderingManager::GetResourcesManager()
{
	return m_resourcesManager;
}

glm::mat4 AmberEngine::Managers::RenderingManager::CalculateProjectionMatrix()
{
	return m_cameraController->GetProjectionMatrix();
}

glm::mat4 AmberEngine::Managers::RenderingManager::CalculateViewMatrix()
{
	return m_cameraController->GetCamera().GetViewMatrix();
}

glm::mat4 AmberEngine::Managers::RenderingManager::GetUnitModelMatrix()
{
	return glm::mat4(1.0f);
}

AmberEngine::LowRenderer::CameraController& AmberEngine::Managers::RenderingManager::GetCameraController()
{
	return *m_cameraController;
}
