#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Rendering/Settings/EProjectionMode.h"

namespace AmberEngine::Rendering::Entities
{
	class API_AMBERENGINE Camera
	{
	public:
		Camera();
		~Camera() = default;

		void ComputeMatrices(uint16_t p_windowWidth, uint16_t p_windowHeight, const glm::vec3& p_position, const glm::quat& p_rotation);

		void SetProjectionMode(Settings::EProjectionMode p_projectionMode);

		void SetSize(float p_value);
		void SetFov(float p_value);
		void SetNear(float p_value);
		void SetFar(float p_value);

		void SetClearColor(const glm::vec3& p_clearColor);

		Settings::EProjectionMode GetProjectionMode() const;

		float GetFov() const;
		float GetSize() const;
		float GetNear() const;
		float GetFar() const;

		const glm::vec3& GetClearColor() const;

		glm::mat4& GetViewMatrix();
		glm::mat4& GetProjectionMatrix();
		
	private:
		void ComputeViewMatrix(const glm::vec3& p_position, const glm::quat& p_rotation);
		void ComputeProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight);

	private:
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		glm::vec3 m_clearColor;

		float m_fov;
		float m_size;
		float m_near;
		float m_far;

		Settings::EProjectionMode m_projectionMode;
	};
}
