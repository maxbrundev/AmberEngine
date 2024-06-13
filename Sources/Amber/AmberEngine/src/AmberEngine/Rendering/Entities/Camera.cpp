#include "Amberpch.h"

#include "AmberEngine/Rendering/Entities/Camera.h"

AmberEngine::Rendering::Entities::Camera::Camera() :
m_clearColor(0.1f, 0.1f, 0.1f),
m_fov(60.0f),
m_near(0.1f),
m_far(100.0f),
m_projectionMode(Rendering::Settings::EProjectionMode::PERSPECTIVE)
{
}

void AmberEngine::Rendering::Entities::Camera::ComputeMatrices(uint16_t p_windowWidth, uint16_t p_windowHeight, const glm::vec3& p_position, const glm::quat& p_rotation)
{
	ComputeViewMatrix(p_position, p_rotation);
	ComputeProjectionMatrix(p_windowWidth, p_windowHeight);
}

void AmberEngine::Rendering::Entities::Camera::SetProjectionMode(Rendering::Settings::EProjectionMode p_projectionMode)
{
	m_projectionMode = p_projectionMode;
}

void AmberEngine::Rendering::Entities::Camera::SetSize(float p_value)
{
	m_size = p_value;
}

glm::mat4& AmberEngine::Rendering::Entities::Camera::GetViewMatrix()
{
	return m_viewMatrix;
}

glm::mat4& AmberEngine::Rendering::Entities::Camera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

void AmberEngine::Rendering::Entities::Camera::SetFov(float p_value)
{
	m_fov = p_value;
}

void AmberEngine::Rendering::Entities::Camera::SetNear(float p_value)
{
	m_near = p_value;
}

void AmberEngine::Rendering::Entities::Camera::SetFar(float p_value)
{
	m_far = p_value;
}

void AmberEngine::Rendering::Entities::Camera::SetClearColor(const glm::vec3& p_clearColor)
{
	m_clearColor = p_clearColor;
}

AmberEngine::Rendering::Settings::EProjectionMode AmberEngine::Rendering::Entities::Camera::GetProjectionMode() const
{
	return m_projectionMode;
}

float& AmberEngine::Rendering::Entities::Camera::GetCameraFov()
{
	return m_fov;
}

const glm::vec3& AmberEngine::Rendering::Entities::Camera::GetClearColor() const
{
	return m_clearColor;
}

void AmberEngine::Rendering::Entities::Camera::ComputeViewMatrix(const glm::vec3& p_position, const glm::quat& p_rotation)
{
	const auto& up = p_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	const auto& forward = p_rotation * glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewMatrix = glm::lookAt(p_position, p_position + forward, up);
}

void AmberEngine::Rendering::Entities::Camera::ComputeProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight)
{
	const auto ratio = p_windowWidth / static_cast<float>(p_windowHeight);

	switch (m_projectionMode)
	{
	case Rendering::Settings::EProjectionMode::PERSPECTIVE:
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), ratio, m_near, m_far);
		break;
	case Rendering::Settings::EProjectionMode::ORTHOGRAPHIC:
		m_projectionMatrix = glm::ortho(m_size, ratio, m_near, m_far);
		break;
	}
}
