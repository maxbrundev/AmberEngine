#include "Amberpch.h"

#include "AmberRendering/Entities/Camera.h"

AmberRendering::Entities::Camera::Camera() :
m_clearColor(0.1f, 0.1f, 0.1f),
m_fov(60.0f),
m_near(0.1f),
m_far(100.0f),
m_projectionMode(Settings::EProjectionMode::PERSPECTIVE),
m_frustumGeometryCulling(false),
m_frustumLightCulling(false)
{
}

void AmberRendering::Entities::Camera::ComputeMatrices(uint16_t p_windowWidth, uint16_t p_windowHeight, const glm::vec3& p_position, const glm::quat& p_rotation)
{
	ComputeViewMatrix(p_position, p_rotation);
	ComputeProjectionMatrix(p_windowWidth, p_windowHeight);
	ComputeFrustum(m_viewMatrix, m_projectionMatrix);
}

void AmberRendering::Entities::Camera::ComputeFrustum(const glm::mat4& p_view, const glm::mat4& p_projection)
{
	m_frustum.CalculateFrustum(p_projection * p_view);
}

void AmberRendering::Entities::Camera::SetProjectionMode(Settings::EProjectionMode p_projectionMode)
{
	m_projectionMode = p_projectionMode;
}

void AmberRendering::Entities::Camera::SetSize(float p_value)
{
	m_size = p_value;
}

glm::mat4& AmberRendering::Entities::Camera::GetViewMatrix()
{
	return m_viewMatrix;
}

glm::mat4& AmberRendering::Entities::Camera::GetProjectionMatrix()
{
	return m_projectionMatrix;
}

void AmberRendering::Entities::Camera::SetFov(float p_value)
{
	m_fov = p_value;
}

void AmberRendering::Entities::Camera::SetNear(float p_value)
{
	m_near = p_value;
}

void AmberRendering::Entities::Camera::SetFar(float p_value)
{
	m_far = p_value;
}

void AmberRendering::Entities::Camera::SetClearColor(const glm::vec3& p_clearColor)
{
	m_clearColor = p_clearColor;
}

void AmberRendering::Entities::Camera::SetFrustumGeometryCulling(bool p_enable)
{
	m_frustumGeometryCulling = p_enable;
}

void AmberRendering::Entities::Camera::SetFrustumLightCulling(bool p_enable)
{
	m_frustumLightCulling = p_enable;
}

bool AmberRendering::Entities::Camera::HasFrustumGeometryCulling() const
{
	return m_frustumGeometryCulling;
}

bool AmberRendering::Entities::Camera::HasFrustumLightCulling() const
{
	return m_frustumLightCulling;
}

const AmberRendering::Data::Frustum& AmberRendering::Entities::Camera::GetFrustum() const
{
	return m_frustum;
}

AmberRendering::Settings::EProjectionMode AmberRendering::Entities::Camera::GetProjectionMode() const
{
	return m_projectionMode;
}

float AmberRendering::Entities::Camera::GetFov() const
{
	return m_fov;
}

float AmberRendering::Entities::Camera::GetSize() const
{
	return m_size;
}

float AmberRendering::Entities::Camera::GetNear() const
{
	return m_near;
}

float AmberRendering::Entities::Camera::GetFar() const
{
	return m_far;
}

const glm::vec3& AmberRendering::Entities::Camera::GetClearColor() const
{
	return m_clearColor;
}

void AmberRendering::Entities::Camera::ComputeViewMatrix(const glm::vec3& p_position, const glm::quat& p_rotation)
{
	const auto& up = p_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
	const auto& forward = p_rotation * glm::vec3(0.0f, 0.0f, 1.0f);
	m_viewMatrix = glm::lookAt(p_position, p_position + forward, up);
}

void AmberRendering::Entities::Camera::ComputeProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight)
{
	const auto ratio = p_windowWidth / static_cast<float>(p_windowHeight);

	switch (m_projectionMode)
	{
	case Settings::EProjectionMode::PERSPECTIVE:
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), ratio, m_near, m_far);
		break;
	case Settings::EProjectionMode::ORTHOGRAPHIC:
		{
			const auto right = m_size * ratio;
			const auto left = -right;

			const auto top = m_size;
			const auto bottom = -top;
			m_projectionMatrix = glm::ortho(left, right, bottom, top, m_near, m_far);
		}
		break;
	}
}
