#include "AmberEngine/LowRenderer/CameraController.h"

#include <glm/gtc/matrix_transform.hpp>

AmberEngine::LowRenderer::CameraController::CameraController(Context::Window& p_window, Inputs::InputManager& p_inputManager, glm::vec3 p_position)
: m_window(p_window), inputManager(p_inputManager), m_camera(p_position), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_isFirstMouse(true), m_isLock(false)
{
}

void AmberEngine::LowRenderer::CameraController::ProcessKeyboard(cameraMovement p_direction, float p_deltaTime)
{
	glm::vec3& position = m_camera.GetPosition();
	glm::vec3 forward	= m_camera.GetForward();
	glm::vec3 right		= m_camera.GetRight();
	glm::vec3 up		= m_camera.GetUp();

	float velocity = m_movementSpeed * p_deltaTime;

	if (p_direction == cameraMovement::FORWARD)
		position += forward * velocity;
	if (p_direction == cameraMovement::BACKWARD)
		position -= forward * velocity;
	if (p_direction == cameraMovement::LEFT)
		position -= right * velocity;
	if (p_direction == cameraMovement::RIGHT)
		position += right * velocity;
	if (p_direction == cameraMovement::UP)
		position += up * velocity;
	if (p_direction == cameraMovement::DOWN)
		position -= up * velocity;
}

void AmberEngine::LowRenderer::CameraController::ProcessMouseMovement(float p_xoffset, float p_yoffset, bool p_isConstrainPitch)
{
	float& yaw = m_camera.GetYaw();
	float& pitch = m_camera.GetPitch();

	p_xoffset *= m_mouseSensitivity;
	p_yoffset *= m_mouseSensitivity;
	
	yaw += p_xoffset;
	pitch += p_yoffset;

	if (p_isConstrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;

		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	m_camera.UpdateCameraVectors();
}

void AmberEngine::LowRenderer::CameraController::ProcessMouseScroll(float p_yoffset)
{
	float& fov = m_camera.GetCameraFov();

	if (fov >= 1.0f && fov <= 45.0f)
		fov -= p_yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void AmberEngine::LowRenderer::CameraController::Update(float p_deltaTime)
{
	if (!m_isLock)
	{
		HandleInput(p_deltaTime);
		HandleMouse();
	}
}

void AmberEngine::LowRenderer::CameraController::HandleInput(float p_deltaTime)
{
	if (inputManager.GetKey(Inputs::EKey::KEY_W) == Inputs::EKeyState::KEY_DOWN)
		ProcessKeyboard(cameraMovement::FORWARD, p_deltaTime);
	if (inputManager.GetKey(Inputs::EKey::KEY_S) == Inputs::EKeyState::KEY_DOWN)
		ProcessKeyboard(cameraMovement::BACKWARD, p_deltaTime);
	if (inputManager.GetKey(Inputs::EKey::KEY_A) == Inputs::EKeyState::KEY_DOWN)
		ProcessKeyboard(cameraMovement::LEFT, p_deltaTime);
	if (inputManager.GetKey(Inputs::EKey::KEY_D) == Inputs::EKeyState::KEY_DOWN)
		ProcessKeyboard(cameraMovement::RIGHT, p_deltaTime);
	if (inputManager.GetKey(Inputs::EKey::KEY_SPACE) == Inputs::EKeyState::KEY_DOWN)
		ProcessKeyboard(cameraMovement::UP, p_deltaTime);
	if (inputManager.GetKey(Inputs::EKey::KEY_LEFT_CONTROL) == Inputs::EKeyState::KEY_DOWN)
		ProcessKeyboard(cameraMovement::DOWN, p_deltaTime);
}

void AmberEngine::LowRenderer::CameraController::HandleMouse()
{
	double xPos;
	double yPos;

	glfwGetCursorPos(m_window.GetGlfwWindow(), &xPos, &yPos);

	if (m_isFirstMouse)
	{
		m_lastMousePosX = xPos;
		m_lastMousePosY = yPos;
		m_isFirstMouse = false;
	}

	float xoffset = xPos - m_lastMousePosX;
	float yoffset = m_lastMousePosY - yPos;

	m_lastMousePosX = xPos;
	m_lastMousePosY = yPos;

	ProcessMouseMovement(xoffset, yoffset);
}

void AmberEngine::LowRenderer::CameraController::Lock()
{
	m_isLock = true;
	m_isFirstMouse = true;
}

void AmberEngine::LowRenderer::CameraController::Unlock()
{
	m_isLock = false;
}

glm::mat4 AmberEngine::LowRenderer::CameraController::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(m_camera.GetCameraFov()), static_cast<float>(m_window.GetSize().first) / static_cast<float>(m_window.GetSize().second), 0.1f, 300.0f);
}

AmberEngine::LowRenderer::Camera& AmberEngine::LowRenderer::CameraController::GetCamera()
{
	return m_camera;
}
