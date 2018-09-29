#include "pch.h"

#include "LowRenderer/Camera.h"

RenderEngine::LowRenderer::Camera::Camera(Core::Context& p_context, glm::vec3 p_position, glm::vec3 p_up, float p_yaw, float p_pitch)
	: m_context(p_context), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_zoom(ZOOM), m_firstMouse(true), m_isLock(false)
{
	m_position = p_position;
	m_worldUp = p_up;
	m_yaw = p_yaw;
	m_pitch = p_pitch;

	UpdateCameraVectors();

	m_lastMousePosX = m_context.GetWidthWindow() / 2.0f;
	m_lastMousePosY = m_context.GetHeightWindow() / 2.0f;
}

RenderEngine::LowRenderer::Camera::Camera(Core::Context& p_context, float p_posX, float p_posY, float p_posZ, float p_upX, float p_upY, float p_upZ, float p_yaw, float p_pitch)
	: m_context(p_context), m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVITY), m_zoom(ZOOM), m_firstMouse(true), m_isLock(false)
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
		m_position += m_front * velocity;
	if (p_direction == cameraMovement::BACKWARD)
		m_position -= m_front * velocity;
	if (p_direction == cameraMovement::LEFT)
		m_position -= m_right * velocity;
	if (p_direction == cameraMovement::RIGHT)
		m_position += m_right * velocity;
}

void RenderEngine::LowRenderer::Camera::ProcessMouseMovement(GLdouble p_xoffset, GLdouble p_yoffset, GLboolean p_constrainPitch)
{
	p_xoffset *= m_mouseSensitivity;
	p_yoffset *= m_mouseSensitivity;

	m_yaw += p_xoffset;
	m_pitch += p_yoffset;

	if (p_constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;

		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	UpdateCameraVectors();
}

void RenderEngine::LowRenderer::Camera::ProcessMouseScroll(GLdouble p_yoffset)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= p_yoffset;
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f;
}

void RenderEngine::LowRenderer::Camera::Update(GLdouble p_deltaTime)
{
	HandleInput(p_deltaTime);
	HandleMouse();
}

void RenderEngine::LowRenderer::Camera::HandleInput(GLdouble p_deltaTime)
{
	if (glfwGetKey(m_context.GetContextWindow(), GLFW_KEY_LEFT_ALT))
	{
		m_isLock = true;
		m_firstMouse = true;
	}
	else
		m_isLock = false;

	if(!m_isLock)
	{
		if (glfwGetKey(m_context.GetContextWindow(), GLFW_KEY_W) == GLFW_PRESS)
			ProcessKeyboard(cameraMovement::FORWARD, p_deltaTime);
		if (glfwGetKey(m_context.GetContextWindow(), GLFW_KEY_S) == GLFW_PRESS)
			ProcessKeyboard(cameraMovement::BACKWARD, p_deltaTime);
		if (glfwGetKey(m_context.GetContextWindow(), GLFW_KEY_A) == GLFW_PRESS)
			ProcessKeyboard(cameraMovement::LEFT, p_deltaTime);
		if (glfwGetKey(m_context.GetContextWindow(), GLFW_KEY_D) == GLFW_PRESS)
			ProcessKeyboard(cameraMovement::RIGHT, p_deltaTime);
	}
}

void RenderEngine::LowRenderer::Camera::HandleMouse()
{
	if (!m_isLock)
	{
		GLdouble xPos;
		GLdouble yPos;

		glfwGetCursorPos(m_context.GetContextWindow(), &xPos, &yPos);

		if (m_firstMouse)
		{
			m_lastMousePosX = xPos;
			m_lastMousePosY = yPos;
			m_firstMouse = false;
		}

		const GLdouble xoffset = xPos - m_lastMousePosX;
		const GLdouble yoffset = m_lastMousePosY - yPos;

		m_lastMousePosX = xPos;
		m_lastMousePosY = yPos;

		ProcessMouseMovement(xoffset, yoffset);
	}
}

glm::mat4 RenderEngine::LowRenderer::Camera::GetViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_front, m_up);
}

float RenderEngine::LowRenderer::Camera::GetCameraZoom() const
{
	return m_zoom;
}

void RenderEngine::LowRenderer::Camera::UpdateCameraVectors()
{
	glm::vec3 front;

	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_front = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_front));
}