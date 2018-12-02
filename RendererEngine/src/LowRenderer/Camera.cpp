#include "pch.h"

#include "LowRenderer/Camera.h"

RenderEngine::LowRenderer::Camera::Camera(Core::Device& p_context, glm::vec3 p_position, glm::vec3 p_up, float p_yaw, float p_pitch)
	: m_device(p_context), m_forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_fov(FOV), m_isFirstMouse(true), m_isLock(false)
{
	m_position = p_position;
	m_worldUp = p_up;
	m_yaw = p_yaw;
	m_pitch = p_pitch;

	UpdateCameraVectors();

	m_lastMousePosX = m_device.GetWindowWidth() / 2.0f;
	m_lastMousePosY = m_device.GetWindowHeight() / 2.0f;
}

RenderEngine::LowRenderer::Camera::Camera(Core::Device& p_context, float p_posX, float p_posY, float p_posZ, float p_upX, float p_upY, float p_upZ, float p_yaw, float p_pitch)
	: m_device(p_context), m_forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_fov(FOV), m_isFirstMouse(true), m_isLock(false)
{
	m_position = glm::vec3(p_posX, p_posY, p_posZ);
	m_worldUp = glm::vec3(p_upX, p_upY, p_upZ);
	m_yaw = p_yaw;
	m_pitch = p_pitch;

	UpdateCameraVectors();
}

void RenderEngine::LowRenderer::Camera::ProcessKeyboard(cameraMovement p_direction, float p_deltaTime)
{
	float velocity = m_movementSpeed * p_deltaTime;

	if (p_direction == cameraMovement::FORWARD)
		m_position += m_forward * velocity;
	if (p_direction == cameraMovement::BACKWARD)
		m_position -= m_forward * velocity;
	if (p_direction == cameraMovement::LEFT)
		m_position -= m_right * velocity;
	if (p_direction == cameraMovement::RIGHT)
		m_position += m_right * velocity;
}

void RenderEngine::LowRenderer::Camera::ProcessMouseMovement(float p_xoffset, float p_yoffset, bool p_isConstrainPitch)
{
	p_xoffset *= m_mouseSensitivity;
	p_yoffset *= m_mouseSensitivity;

	m_yaw += p_xoffset;
	m_pitch += p_yoffset;

	if (p_isConstrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;

		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void RenderEngine::LowRenderer::Camera::ProcessMouseScroll(float p_yoffset)
{
	if (m_fov >= 1.0f && m_fov <= 45.0f)
		m_fov -= p_yoffset;
	if (m_fov <= 1.0f)
		m_fov = 1.0f;
	if (m_fov >= 45.0f)
		m_fov = 45.0f;
}

void RenderEngine::LowRenderer::Camera::Update(float p_deltaTime)
{
	HandleInput(p_deltaTime);
	HandleMouse();
}

void RenderEngine::LowRenderer::Camera::HandleInput(float p_deltaTime)
{
	if (glfwGetKey(m_device.GetContextWindow(), GLFW_KEY_LEFT_ALT))
	{
		m_isLock = true;
		m_isFirstMouse = true;
	}
	else
		m_isLock = false;

	if(!m_isLock)
	{
		if (glfwGetKey(m_device.GetContextWindow(), GLFW_KEY_W) == GLFW_PRESS)
			ProcessKeyboard(cameraMovement::FORWARD, p_deltaTime);
		if (glfwGetKey(m_device.GetContextWindow(), GLFW_KEY_S) == GLFW_PRESS)
			ProcessKeyboard(cameraMovement::BACKWARD, p_deltaTime);
		if (glfwGetKey(m_device.GetContextWindow(), GLFW_KEY_A) == GLFW_PRESS)
			ProcessKeyboard(cameraMovement::LEFT, p_deltaTime);
		if (glfwGetKey(m_device.GetContextWindow(), GLFW_KEY_D) == GLFW_PRESS)
			ProcessKeyboard(cameraMovement::RIGHT, p_deltaTime);
	}
}

void RenderEngine::LowRenderer::Camera::HandleMouse()
{
	if (!m_isLock)
	{
		GLdouble xPos;
		GLdouble yPos;

		glfwGetCursorPos(m_device.GetContextWindow(), &xPos, &yPos);

		if (m_isFirstMouse)
		{
			m_lastMousePosX = xPos;
			m_lastMousePosY = yPos;
			m_isFirstMouse = false;
		}

		GLdouble xoffset = xPos - m_lastMousePosX;
		GLdouble yoffset = m_lastMousePosY - yPos;

		m_lastMousePosX = xPos;
		m_lastMousePosY = yPos;

		ProcessMouseMovement(xoffset, yoffset);
	}
}

glm::vec3 RenderEngine::LowRenderer::Camera::GetPosition() const
{
	return m_position;
}

glm::mat4 RenderEngine::LowRenderer::Camera::GetProjectionMatrix() const
{
	return glm::perspective(glm::radians(m_fov), static_cast<float>(m_device.GetWindowWidth()) / static_cast<float>(m_device.GetWindowHeight()), 0.1f, 100.0f);;
}

glm::mat4 RenderEngine::LowRenderer::Camera::GetViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_forward, m_up);;
}

float RenderEngine::LowRenderer::Camera::GetCameraFov() const
{
	return m_fov;
}

void RenderEngine::LowRenderer::Camera::UpdateCameraVectors()
{
	glm::vec3 front;

	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_forward = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_forward));
}