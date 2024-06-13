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
		CameraController(Rendering::Entities::Camera& p_camera, glm::vec3& p_position, glm::quat& p_rotation);
		~CameraController() = default;

		void Update(float p_deltaTime);
		
		void SetPosition(const glm::vec3& p_position) const;
		void SetPosition(float p_posX, float p_posY, float p_posZ) const;

		void SetRotation(const glm::quat& p_rotation) const;
		const glm::quat& GetRotation() const;
		void SetSpeed(float p_value);
		float GetSpeed() const;
		const glm::vec3& GetPosition() const;

	private:
		void HandleInputs(float p_deltaTime);
		void HandleMouse();
		void HandleCameraZoom() const;

	private:
		AmberEngine::Context::Window& m_window;
		AmberEngine::Inputs::InputManager& m_inputManager;

		Rendering::Entities::Camera& m_camera;
		glm::vec3 m_eulerRotation;
		glm::vec3& m_position;
		glm::quat& m_rotation;

		double m_lastMousePosX = 0.0f;
		double m_lastMousePosY = 0.0f;

		float m_mouseSensitivity = 0.12f;
		float m_moveSpeed = 15.0f;

		bool m_isFirstMouse = true;
		bool m_rightMousePressed = false;

		glm::vec3 m_targetPosition;
		glm::vec3 m_currentMovement;
	};
}