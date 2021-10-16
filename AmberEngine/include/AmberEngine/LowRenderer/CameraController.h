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
		CameraController(Context::Window& p_window, Inputs::InputManager& p_inputManager, glm::vec3 p_position = glm::vec3(0.0f, 0.0f, -3.0f));
		~CameraController() = default;

		void ProcessKeyboard(cameraMovement p_direction, float p_deltaTime);
		void ProcessMouseMovement(float p_xoffset, float p_yoffset, bool p_isConstrainPitch = true);
		void ProcessMouseScroll(float p_yoffset);

		void Update(float p_deltaTime);
		
		void SetPosition(glm::vec3 p_pos);
		void SetPosition(float pos_x, float pos_y, float pos_z);
		
		const glm::vec3& GetPosition() const;
		
		void HandleInput(float p_deltaTime);
		void HandleMouse();
		void Lock();
		void Unlock();

		Camera& GetCamera();

	private:
		const float SPEED = 10.5;
		const float SENSITIVITY = 0.1f;

		Context::Window& m_window;
		Inputs::InputManager& inputManager;
		Camera m_camera;

		double m_lastMousePosX;
		double m_lastMousePosY;
		float m_movementSpeed;
		float m_mouseSensitivity;

		bool m_isFirstMouse;
		bool m_isLock;

		glm::vec3 m_position;

	};
}