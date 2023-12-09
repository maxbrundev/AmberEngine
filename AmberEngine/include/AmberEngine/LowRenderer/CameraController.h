#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Window.h"

#include "AmberEngine/Inputs/InputManager.h"

#include "AmberEngine/LowRenderer/Camera.h"

namespace AmberEngine::LowRenderer
{
	class API_AMBERENGINE CameraController
	{
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
		CameraController(Camera& p_camera,  glm::vec3& p_position);
		~CameraController() = default;

		void ProcessKeyboard(cameraMovement p_direction, float p_deltaTime);
		void ProcessMouseMovement(float p_offsetX, float p_offsetY);
		void ProcessMouseScroll(float p_offsetY);

		void Update(float p_deltaTime);
		
		void SetPosition(const glm::vec3& p_position);
		void SetPosition(float p_posX, float p_posY, float p_posZ);
		
		const glm::vec3& GetPosition() const;
		
		void HandleInputs(float p_deltaTime);
		void HandleMouse();

	private:
		Context::Window& m_window;
		Inputs::InputManager& m_inputManager;

		Camera& m_camera;

		glm::vec3& m_position;

		double m_lastMousePosX = 0.0f;
		double m_lastMousePosY = 0.0f;

		float m_mouseSensitivity = 0.1f;
		float m_moveSpeed = 10.0f;

		bool m_isFirstMouse = true;
		bool m_rightMousePressed = false;
	};
}