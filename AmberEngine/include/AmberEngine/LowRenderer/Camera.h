#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::LowRenderer
{
	class API_AMBERENGINE Camera
	{
	public:
		Camera();
		~Camera() = default;
		
		void UpdateCameraVectors();
		void CalculateMatrices(uint16_t p_windowWidth, uint16_t p_windowHeight, const glm::vec3& p_position);
		
		void SetFov(float p_value);
		void SetNear(float p_value);
		void SetFar(float p_value);

		void SetClearColor(const glm::vec3& p_clearColor);

		float& GetYaw();
		float& GetPitch();
		float& GetCameraFov();
		
		const glm::vec3& GetForward() const;
		const glm::vec3& GetRight() const;
		const glm::vec3& GetUp() const;

		const glm::vec3& GetClearColor() const;

		glm::mat4& GetViewMatrix();
		glm::mat4& GetProjectionMatrix();
		
	private:
		void CalculateViewMatrix(const glm::vec3& p_position, const  glm::vec3& p_up);
		void CalculateProjectionMatrix(uint16_t p_windowWidth, uint16_t p_windowHeight);

	private:
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projectionMatrix;

		glm::vec3 m_forward;
		glm::vec3 m_up;
		glm::vec3 m_right;
		
		glm::vec3 m_clearColor;

		float m_yaw;
		float m_pitch;
		float m_fov;

		float m_near;
		float m_far;
	};
}
