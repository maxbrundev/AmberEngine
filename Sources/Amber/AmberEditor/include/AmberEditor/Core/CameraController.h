#pragma once


#include <queue>

#include "AmberWindowing/Context/Window.h"

#include "AmberWindowing/Inputs/InputManager.h"

#include "AmberRendering/Entities/Camera.h"

namespace AmberCore::ECS
{
	class Actor;
}

namespace AmberEditor::Core
{
	class CameraController
	{
	public:
		CameraController(AmberRendering::Entities::Camera& p_camera, glm::vec3& p_position, glm::quat& p_rotation, bool p_enableFocusInputs = false);
		~CameraController() = default;

		void Update(float p_deltaTime);

		void MoveToTarget(AmberCore::ECS::Actor& p_target);

		void SetPosition(const glm::vec3& p_position) const;
		void SetPosition(float p_posX, float p_posY, float p_posZ) const;

		void SetRotation(const glm::quat& p_rotation) const;
		const glm::quat& GetRotation() const;
		void SetSpeed(float p_value);
		float GetSpeed() const;
		const glm::vec3& GetPosition() const;
		bool IsRightMousePressed() const;
	private:
		void HandleFocusInputs();
		void HandleCameraDestinations(float p_deltaTime);
		void HandleInputs(float p_deltaTime);
		void HandleMouse();
		void HandleCameraZoom() const;

	private:
		AmberWindowing::Context::Window& m_window;
		AmberWindowing::Inputs::InputManager& m_inputManager;

		AmberRendering::Entities::Camera& m_camera;
		glm::vec3 m_eulerRotation;
		glm::vec3& m_position;
		glm::quat& m_rotation;

		std::queue<std::tuple<glm::vec3, glm::quat>> m_cameraDestinations;

		bool m_enableFocusInputs;

		double m_lastMousePosX = 0.0f;
		double m_lastMousePosY = 0.0f;

		float m_mouseSensitivity = 0.12f;
		float m_moveSpeed = 15.0f;
		float m_focusLerpCoefficient = 6.0f;

		bool m_isFirstMouse = true;
		bool m_isFirstMiddleMouse = true;
		bool m_rightMousePressed = false;
		bool m_middleMousePressed = false;

		glm::vec3 m_targetPosition;
		glm::vec3 m_currentMovement;
	};
}
