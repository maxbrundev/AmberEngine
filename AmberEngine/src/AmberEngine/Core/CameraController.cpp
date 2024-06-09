#include "Amberpch.h"

#include "AmberEngine/Core/CameraController.h"

#include <glm/gtx/compatibility.hpp>

#include "AmberEngine/Tools/Global/ServiceLocator.h"

AmberEngine::Core::CameraController::CameraController(Rendering::Entities::Camera& p_camera, glm::vec3& p_position) :
	m_window(Tools::Global::ServiceLocator::Get<Context::Window>()),
	m_inputManager(Tools::Global::ServiceLocator::Get<Inputs::InputManager>()),
	m_camera(p_camera),
	m_position(p_position),
	m_targetPosition{},
	m_currentMovement{}
{
}

void AmberEngine::Core::CameraController::Update(float p_deltaTime)
{
	HandleInputs(p_deltaTime);
	HandleMouse();
	HandleCameraZoom();

	if (m_inputManager.IsMouseButtonPressed(Inputs::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		m_rightMousePressed = true;
		m_window.SetCursorMode(Context::ECursorMode::DISABLED);
	}

	if (m_inputManager.IsMouseButtonReleased(Inputs::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		m_isFirstMouse = true;
		m_rightMousePressed = false;
		m_window.SetCursorMode(Context::ECursorMode::NORMAL);
	}

	auto& io = ImGui::GetIO();
	io.DisableMouseUpdate = m_rightMousePressed;
}

void AmberEngine::Core::CameraController::SetPosition(const glm::vec3& p_position)
{
	m_position = p_position;
}

void AmberEngine::Core::CameraController::SetPosition(float p_posX, float p_posY, float p_posZ)
{
	m_position.x = p_posX;
	m_position.y = p_posY;
	m_position.z = p_posZ;
}

const glm::vec3& AmberEngine::Core::CameraController::GetPosition() const
{
	return m_position;
}

void AmberEngine::Core::CameraController::HandleInputs(float p_deltaTime)
{
	m_targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	float velocity = m_moveSpeed * p_deltaTime;

	if (m_rightMousePressed) 
	{
		if (m_inputManager.GetKey(Inputs::EKey::KEY_W) == Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_camera.GetForward() * velocity;
		}

		if (m_inputManager.GetKey(Inputs::EKey::KEY_A) == Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_camera.GetRight() * -velocity;
		}

		if (m_inputManager.GetKey(Inputs::EKey::KEY_S) == Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_camera.GetForward() * -velocity;
		}

		if (m_inputManager.GetKey(Inputs::EKey::KEY_D) == Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_camera.GetRight() * velocity;
		}

		if (m_inputManager.GetKey(Inputs::EKey::KEY_E) == Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += glm::vec3(0.0f, velocity, 0.0f);
		}

		if (m_inputManager.GetKey(Inputs::EKey::KEY_Q) == Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += glm::vec3(0.0f, -velocity, 0.0f);
		}
	}

	m_currentMovement = glm::lerp(m_currentMovement, m_targetPosition, 10.0f * p_deltaTime);
	m_position += m_currentMovement;
}

void AmberEngine::Core::CameraController::HandleMouse()
{
	if (m_rightMousePressed)
	{
		float& yaw   = m_camera.GetYaw();
		float& pitch = m_camera.GetPitch();

		auto[xPos, yPos] = m_inputManager.GetMousePosition();

		if (m_isFirstMouse)
		{
			m_lastMousePosX = xPos;
			m_lastMousePosY = yPos;
			m_isFirstMouse = false;
		}

		yaw   += (xPos - m_lastMousePosX) * m_mouseSensitivity;
		pitch += (m_lastMousePosY - yPos) * m_mouseSensitivity;

		pitch = std::clamp(pitch, -89.0f, 89.0f);

		m_camera.UpdateCameraVectors();

		m_lastMousePosX = xPos;
		m_lastMousePosY = yPos;
	}
}

void AmberEngine::Core::CameraController::HandleCameraZoom()
{
	m_position += m_camera.GetForward() * ImGui::GetIO().MouseWheel;
}
