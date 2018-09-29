#pragma once

#include "Context/Context.h"

namespace RenderEngine
{
	namespace LowRenderer
	{
		const float YAW = -90.0f;
		const float PITCH = 0.0f;
		const float SPEED = 2.5f;
		const float SENSITIVITY = 0.1f;
		const float ZOOM = 45.0f;

		class Camera
		{
		private:
			enum class cameraMovement
			{
				FORWARD,
				BACKWARD,
				LEFT,
				RIGHT
			};

			Core::Context& m_context;

			glm::vec3 m_front;
			glm::vec3 m_up;
			glm::vec3 m_right;
			glm::vec3 m_worldUp;

			GLdouble m_yaw;
			GLdouble m_pitch;

			GLdouble m_lastMousePosX;
			GLdouble m_lastMousePosY;

			GLdouble m_movementSpeed;
			GLdouble m_mouseSensitivity;

			float m_zoom;

			bool m_firstMouse;
			bool m_isLock;

		public:
			glm::vec3 m_position;

		public:
			Camera(Core::Context& p_context, glm::vec3 p_position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 p_up = glm::vec3(0.0f, 1.0f, 0.0f), float p_yaw = YAW, float p_pitch = PITCH);
			Camera(Core::Context& p_context, float p_posX, float p_posY, float p_posZ, float p_upX, float p_upY, float p_upZ, float p_yaw, float p_pitch);

			void ProcessKeyboard(cameraMovement p_direction, float p_deltaTime);
			void ProcessMouseMovement(GLdouble p_xoffset, GLdouble p_yoffset, GLboolean p_constrainPitch = true);
			void ProcessMouseScroll(GLdouble p_yoffset);

			void Update(GLdouble p_deltaTime);

			void HandleInput(GLdouble p_deltaTime);
			void HandleMouse();

			glm::mat4 GetViewMatrix() const;
			float GetCameraZoom() const;
				
		private:
			void UpdateCameraVectors();
		};
	}
}