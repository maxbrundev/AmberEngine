#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Context/Window.h"

#include "AmberEngine/Inputs/InputManager.h"

#include "AmberEngine/Rendering/Entities/Camera.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE CameraController
	{
	public:
		CameraController(Rendering::Entities::Camera& p_camera,  glm::vec3& p_position);
		~CameraController() = default;

		void Update(float p_deltaTime);
		
		void SetPosition(const glm::vec3& p_position);
		void SetPosition(float p_posX, float p_posY, float p_posZ);
		
		const glm::vec3& GetPosition() const;

	private:
		void HandleInputs(float p_deltaTime);
		void HandleMouse();
		void HandleCameraZoom();

	private:
		AmberEngine::Context::Window& m_window;
		AmberEngine::Inputs::InputManager& m_inputManager;

		Rendering::Entities::Camera& m_camera;

		glm::vec3& m_position;

		double m_lastMousePosX = 0.0f;
		double m_lastMousePosY = 0.0f;

		float m_mouseSensitivity = 0.1f;
		float m_moveSpeed = 10.0f;

		bool m_isFirstMouse = true;
		bool m_rightMousePressed = false;

		glm::vec3 m_targetPosition;
		glm::vec3 m_currentMovement;
	};
}