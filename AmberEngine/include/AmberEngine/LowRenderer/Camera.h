#pragma once

#include <glm/glm.hpp>

#include "AmberEngine/API/Export.h"

namespace AmberEngine::LowRenderer
{
	class API_AMBERENGINE Camera
	{
	private:
		const float PITCH = 0.0f;
		const float YAW = -90.0f;
		const float FOV = 45.0f;

		glm::vec3 m_position;
		glm::vec3 m_forward;
		glm::vec3 m_up;
		glm::vec3 m_right;

		glm::vec3 m_worldUp;

		float m_yaw;
		float m_pitch;
		float m_fov;

	public:
		Camera(glm::vec3 p_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 p_up = glm::vec3(0.0f, 1.0f, 0.0f));
		//Camera(float p_posX, float p_posY, float p_posZ, float p_upX, float p_upY, float p_upZ, float p_yaw, float p_pitch);

		glm::mat4 GetViewMatrix() const;

		glm::vec3& GetPosition();

		float& GetYaw();
		float& GetPitch();

		const glm::vec3& GetForward() const;
		const glm::vec3& GetRight() const;
		const glm::vec3& GetUp() const;

		float& GetCameraFov();

		void SetPosition(glm::vec3 p_pos);
		void SetPosition(float pos_x, float pos_y, float pos_z);
		void SetFov(float p_value);

		void UpdateCameraVectors();
	};
}
