#include "Amberpch.h"

#include "AmberEngine/Core/CameraController.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include "AmberEngine/Tools/Global/ServiceLocator.h"

AmberEngine::Core::CameraController::CameraController(Rendering::Entities::Camera& p_camera, glm::vec3& p_position, glm::quat& p_rotation) :
	m_window(Tools::Global::ServiceLocator::Get<Context::Window>()),
	m_inputManager(Tools::Global::ServiceLocator::Get<Inputs::InputManager>()),
	m_camera(p_camera),
	m_position(p_position),
	m_rotation(p_rotation),
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

	if (m_inputManager.IsMouseButtonPressed(Inputs::EMouseButton::MOUSE_BUTTON_3))
	{
		m_middleMousePressed = true;
		m_window.SetCursorMode(Context::ECursorMode::DISABLED);
	}

	if (m_inputManager.IsMouseButtonReleased(Inputs::EMouseButton::MOUSE_BUTTON_3))
	{
		m_isFirstMiddleMouse = true;
		m_middleMousePressed = false;
		m_window.SetCursorMode(Context::ECursorMode::NORMAL);
	}

	auto& io = ImGui::GetIO();
	io.DisableMouseUpdate = m_rightMousePressed || m_middleMousePressed;
}

void AmberEngine::Core::CameraController::SetPosition(const glm::vec3& p_position) const
{
	m_position = p_position;
}

void AmberEngine::Core::CameraController::SetPosition(float p_posX, float p_posY, float p_posZ) const
{
	m_position.x = p_posX;
	m_position.y = p_posY;
	m_position.z = p_posZ;
}

void AmberEngine::Core::CameraController::SetRotation(const glm::quat& p_rotation) const
{
	m_rotation = p_rotation;
}

const glm::quat& AmberEngine::Core::CameraController::GetRotation() const
{
	return m_rotation;
}

void AmberEngine::Core::CameraController::SetSpeed(float p_value)
{
	m_moveSpeed = p_value;
}

float AmberEngine::Core::CameraController::GetSpeed() const
{
	return m_moveSpeed;
}

const glm::vec3& AmberEngine::Core::CameraController::GetPosition() const
{
	return m_position;
}

bool AmberEngine::Core::CameraController::IsRightMousePressed() const
{
	return m_rightMousePressed;
}

void AmberEngine::Core::CameraController::HandleInputs(float p_deltaTime)
{
	m_targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	if (m_rightMousePressed) 
	{
		bool run = m_inputManager.GetKey(Inputs::EKey::KEY_LEFT_SHIFT) == Inputs::EKeyState::KEY_DOWN;

		float velocity = m_moveSpeed * p_deltaTime * (run ? 2.0f : 1.0f);

		if (m_inputManager.GetKey(Inputs::EKey::KEY_W) == Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_rotation * glm::vec3(0.0f, 0.0f, 1.0f) * velocity;
		}

		if (m_inputManager.GetKey(Inputs::EKey::KEY_A) == Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_rotation * glm::vec3(1.0f, 0.0f, 0.0f) * velocity;
		}

		if (m_inputManager.GetKey(Inputs::EKey::KEY_S) == Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_rotation * glm::vec3(0.0f, 0.0f, 1.0f) * -velocity;
		}

		if (m_inputManager.GetKey(Inputs::EKey::KEY_D) == Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_rotation * glm::vec3(1.0f, 0.0f, 0.0f) * -velocity;
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

glm::vec3 RemoveRoll(const glm::vec3& p_eulerRotation)
{
	glm::vec3 result = p_eulerRotation;

	if (result.z >= 179.0f || result.z <= -179.0f)
	{
		result.x += result.z;
		result.y = 180.0f - result.y;
		result.z = 0.0f;
	}

	if (result.x > 180.0f)  result.x -= 360.0f;
	if (result.x < -180.0f) result.x += 360.0f;

	return result;
}

void AmberEngine::Core::CameraController::HandleMouse()
{
	if (m_rightMousePressed)
	{
		auto[xPos, yPos] = m_inputManager.GetMousePosition();

		if (m_isFirstMouse)
		{
			m_lastMousePosX = xPos;
			m_lastMousePosY = yPos;
			m_isFirstMouse = false;

			m_eulerRotation = glm::degrees(glm::eulerAngles(m_rotation));

			m_eulerRotation = RemoveRoll(m_eulerRotation);
		}
		
		glm::vec2 mouseOffset
		{
			static_cast<float>(xPos - m_lastMousePosX),
			static_cast<float>(m_lastMousePosY - yPos)
		};

		m_lastMousePosX = xPos;
		m_lastMousePosY = yPos;

		auto mouseOffsetFinal = mouseOffset * m_mouseSensitivity;
		
		m_eulerRotation.y -= mouseOffsetFinal.x;
		m_eulerRotation.x -= mouseOffsetFinal.y;
		m_eulerRotation.x = std::max(std::min(m_eulerRotation.x, 90.0f), -90.0f);

		m_rotation = glm::qua(glm::radians(m_eulerRotation));
	}

	if (m_middleMousePressed)
	{
		auto[xPos, yPos] = m_inputManager.GetMousePosition();

		if (m_isFirstMiddleMouse)
		{
			m_lastMousePosX = xPos;
			m_lastMousePosY = yPos;
			m_isFirstMiddleMouse = false;
		}

		glm::vec2 mouseOffset
		{
			static_cast<float>(xPos - m_lastMousePosX),
			static_cast<float>(m_lastMousePosY - yPos)
		};

		m_lastMousePosX = xPos;
		m_lastMousePosY = yPos;

		auto mouseOffsetFinal = mouseOffset * 0.01f;

		glm::vec3 right = m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
		glm::vec3 up = m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);

		glm::vec3 movement = (right * mouseOffsetFinal.x + -up * mouseOffsetFinal.y);

		m_position += movement;
	}
}

void AmberEngine::Core::CameraController::HandleCameraZoom() const
{
	m_position += m_rotation * glm::vec3(0.0f, 0.0f, 1.0f) * ImGui::GetIO().MouseWheel;
}
