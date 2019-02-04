#pragma once

#include "AmberEngine/Context/Device.h"
#include "AmberEngine/LowRenderer/Camera.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::LowRenderer
{
	class API_AMBERENGINE CameraController
	{
	private:
		const float SPEED = 10.5f;
		const float SENSITIVITY = 0.1f;

		Context::Device& m_device;
		Camera m_camera;

		double m_lastMousePosX{};
		double m_lastMousePosY{};
		float m_movementSpeed;
		float m_mouseSensitivity;

		bool m_isFirstMouse;
		bool m_isLock;

		enum class cameraMovement
		{
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT,
			UP,
			DOWN
		};

	public:
		CameraController(Context::Device& p_device, glm::vec3 p_position);
		~CameraController() = default;

		void ProcessKeyboard(cameraMovement p_direction, float p_deltaTime);
		void ProcessMouseMovement(float p_xoffset, float p_yoffset, bool p_isConstrainPitch = true);
		void ProcessMouseScroll(float p_yoffset);

		void Update(float p_deltaTime);

		void HandleInput(float p_deltaTime);
		void HandleMouse();
		void Lock();
		void Unlock();

		glm::mat4 GetProjectionMatrix();

		Camera& GetCamera();
	};
}