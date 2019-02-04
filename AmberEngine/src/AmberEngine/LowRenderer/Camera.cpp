#include "AmberEngine/LowRenderer/Camera.h"

#include <glm/gtc/matrix_transform.hpp>

AmberEngine::LowRenderer::Camera::Camera(glm::vec3 p_position, glm::vec3 p_up)
	: m_yaw(YAW), m_pitch(PITCH), m_fov(FOV), m_position(p_position), m_forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_worldUp(p_up)
{
	UpdateCameraVectors();
}

/*AmberEngine::LowRenderer::Camera::Camera(float p_posX, float p_posY, float p_posZ, float p_upX, float p_upY, float p_upZ, float p_yaw, float p_pitch)
	: m_forward(glm::vec3(0.0f, 0.0f, -1.0f)), m_fov(FOV)
{
	m_position = glm::vec3(p_posX, p_posY, p_posZ);
	m_worldUp = glm::vec3(p_upX, p_upY, p_upZ);
	m_yaw = p_yaw;
	m_pitch = p_pitch;

	UpdateCameraVectors();
}*/

glm::mat4 AmberEngine::LowRenderer::Camera::GetViewMatrix() const
{
	return glm::lookAt(m_position, m_position + m_forward, m_up);
}

const glm::vec3& AmberEngine::LowRenderer::Camera::GetForward() const
{
	return m_forward;
}

glm::vec3& AmberEngine::LowRenderer::Camera::GetPosition()
{
	return m_position;
}

const glm::vec3& AmberEngine::LowRenderer::Camera::GetRight() const
{
	return m_right;
}

const glm::vec3& AmberEngine::LowRenderer::Camera::GetUp() const
{
	return m_up;
}

void AmberEngine::LowRenderer::Camera::SetPosition(glm::vec3 p_pos)
{
	m_position = p_pos;
}

void AmberEngine::LowRenderer::Camera::SetPosition(float pos_x, float pos_y, float pos_z)
{
	m_position = glm::vec3(pos_x, pos_y, pos_z);
}

void AmberEngine::LowRenderer::Camera::SetFov(float p_value)
{
	m_fov = p_value;
}

float& AmberEngine::LowRenderer::Camera::GetCameraFov()
{
	return m_fov;
}

float& AmberEngine::LowRenderer::Camera::GetYaw()
{
	return m_yaw;
}

float& AmberEngine::LowRenderer::Camera::GetPitch()
{
	return m_pitch;
}

void AmberEngine::LowRenderer::Camera::UpdateCameraVectors()
{
	glm::vec3 front;

	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

	m_forward = glm::normalize(front);

	m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
	m_up = glm::normalize(glm::cross(m_right, m_forward));
}