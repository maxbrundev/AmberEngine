#pragma once

#include <glm/glm.hpp>

#include "AmberEngine/Context/Device.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::LowRenderer
{
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 5.5f;
	const float SENSITIVITY = 0.1f;
	const float FOV = 45.0f;

	class API_AMBERENGINE Camera
	{
	private:
		enum class cameraMovement
		{
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT
		};

		Core::Device& m_device;

		glm::vec3 m_forward;
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_worldUp;

		glm::vec3 m_position;

		float m_yaw;
		float m_pitch;
		float m_fov;

		GLdouble m_lastMousePosX;
		GLdouble m_lastMousePosY;
		float m_movementSpeed;
		float m_mouseSensitivity;

		bool m_isFirstMouse;
		bool m_isLock;

	public:
		Camera(Core::Device& p_context, glm::vec3 p_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 p_up = glm::vec3(0.0f, 1.0f, 0.0f), float p_yaw = YAW, float p_pitch = PITCH);
		Camera(Core::Device& p_context, float p_posX, float p_posY, float p_posZ, float p_upX, float p_upY, float p_upZ, float p_yaw, float p_pitch);

		void ProcessKeyboard(cameraMovement p_direction, float p_deltaTime);
		void ProcessMouseMovement(float p_xoffset, float p_yoffset, bool p_isConstrainPitch = true);
		void ProcessMouseScroll(float p_yoffset);

		void Update(float p_deltaTime);

		void HandleInput(float p_deltaTime);
		void HandleMouse();
		void Lock();
		void Unlock();
		float GetCameraFov() const;

		glm::vec3 GetPosition() const;
		glm::mat4 GetProjectionMatrix() const;
		glm::mat4 GetViewMatrix() const;
			
	private:
		void UpdateCameraVectors();
	};
}