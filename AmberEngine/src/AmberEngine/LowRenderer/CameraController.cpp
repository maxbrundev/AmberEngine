#include "Amberpch.h"

#include "AmberEngine/LowRenderer/CameraController.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"

AmberEngine::LowRenderer::CameraController::CameraController(Camera& p_camera, glm::vec3& p_position) :
	m_window(Tools::Global::ServiceLocator::Get<Context::Window>()),
	inputManager(Tools::Global::ServiceLocator::Get<Inputs::InputManager>()),
	m_camera(p_camera), 
	m_position(p_position)
{
	m_camera.SetFov(60.0f);
}

void AmberEngine::LowRenderer::CameraController::ProcessKeyboard(cameraMovement p_direction, float p_deltaTime)
{
	if (m_rightMousePressed)
	{
		const glm::vec3 forward = m_camera.GetForward();
		const glm::vec3 right = m_camera.GetRight();
		const glm::vec3 up = m_camera.GetUp();

		const float velocity = m_moveSpeed * p_deltaTime;

		if (p_direction == cameraMovement::FORWARD)
		{
			m_position += forward * velocity;
		}
		if (p_direction == cameraMovement::BACKWARD)
		{
			m_position -= forward * velocity;
		}
		if (p_direction == cameraMovement::LEFT)
		{
			m_position -= right * velocity;
		}
		if (p_direction == cameraMovement::RIGHT)
		{
			m_position += right * velocity;
		}
		if (p_direction == cameraMovement::UP)
		{
			m_position += up * velocity;
		}
		if (p_direction == cameraMovement::DOWN)
		{
			m_position -= up * velocity;
		}
	}
}

void AmberEngine::LowRenderer::CameraController::ProcessMouseMovement(float p_offsetX, float p_offsetY)
{
	if (m_rightMousePressed) 
	{
		float& yaw = m_camera.GetYaw();
		float& pitch = m_camera.GetPitch();

		p_offsetX *= m_mouseSensitivity;
		p_offsetY *= m_mouseSensitivity;

		yaw += p_offsetX;
		pitch += p_offsetY;

		if (pitch > 89.0f)
		{
			pitch = 89.0f;
		}
		if (pitch < -89.0f)
		{
			pitch = -89.0f;
		}
	}

	m_camera.UpdateCameraVectors();
}

void AmberEngine::LowRenderer::CameraController::ProcessMouseScroll(float p_offsetY)
{
	float& fov = m_camera.GetCameraFov();

	if (fov >= 1.0f && fov <= 45.0f)
	{
		fov -= p_offsetY;
	}

	if (fov <= 1.0f)
	{
		fov = 1.0f;
	}

	if (fov >= 45.0f)
	{
		fov = 45.0f;
	}
}

void AmberEngine::LowRenderer::CameraController::Update(float p_deltaTime)
{
	HandleInputs(p_deltaTime);
	HandleMouse();

	if (inputManager.IsMouseButtonPressed(Inputs::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		m_rightMousePressed = true;
		m_window.SetCursorMode(Context::ECursorMode::DISABLED);
	}

	if (inputManager.IsMouseButtonReleased(Inputs::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		m_isFirstMouse = true;
		m_rightMousePressed = false;
		m_window.SetCursorMode(Context::ECursorMode::NORMAL);
	}

	auto& io = ImGui::GetIO();
	io.DisableMouseUpdate = m_rightMousePressed;
}

void AmberEngine::LowRenderer::CameraController::SetPosition(const glm::vec3& p_position)
{
	m_position = p_position;
}

void AmberEngine::LowRenderer::CameraController::SetPosition(float p_posX, float p_posY, float p_posZ)
{
	m_position.x = p_posX;
	m_position.y = p_posY;
	m_position.z = p_posZ;
}

const glm::vec3& AmberEngine::LowRenderer::CameraController::GetPosition() const
{
	return m_position;
}

void AmberEngine::LowRenderer::CameraController::HandleInputs(float p_deltaTime)
{
	if (m_rightMousePressed) 
	{
		if (inputManager.GetKey(Inputs::EKey::KEY_W) == Inputs::EKeyState::KEY_DOWN)
		{
			ProcessKeyboard(cameraMovement::FORWARD, p_deltaTime);
		}

		if (inputManager.GetKey(Inputs::EKey::KEY_S) == Inputs::EKeyState::KEY_DOWN)
		{
			ProcessKeyboard(cameraMovement::BACKWARD, p_deltaTime);
		}

		if (inputManager.GetKey(Inputs::EKey::KEY_A) == Inputs::EKeyState::KEY_DOWN)
		{
			ProcessKeyboard(cameraMovement::LEFT, p_deltaTime);
		}

		if (inputManager.GetKey(Inputs::EKey::KEY_D) == Inputs::EKeyState::KEY_DOWN)
		{
			ProcessKeyboard(cameraMovement::RIGHT, p_deltaTime);
		}

		if (inputManager.GetKey(Inputs::EKey::KEY_SPACE) == Inputs::EKeyState::KEY_DOWN)
		{
			ProcessKeyboard(cameraMovement::UP, p_deltaTime);
		}

		if (inputManager.GetKey(Inputs::EKey::KEY_LEFT_CONTROL) == Inputs::EKeyState::KEY_DOWN)
		{
			ProcessKeyboard(cameraMovement::DOWN, p_deltaTime);
		}
	}
}

void AmberEngine::LowRenderer::CameraController::HandleMouse()
{
	if (m_rightMousePressed)
	{
		double posX;
		double posY;

		glfwGetCursorPos(m_window.GetGlfwWindow(), &posX, &posY);

		if (m_isFirstMouse)
		{
			m_lastMousePosX = posX;
			m_lastMousePosY = posY;
			m_isFirstMouse = false;
		}

		const float offsetX = posX - m_lastMousePosX;
		const float offsetY = m_lastMousePosY - posY;

		m_lastMousePosX = posX;
		m_lastMousePosY = posY;

		ProcessMouseMovement(offsetX, offsetY);
	}
}

AmberEngine::LowRenderer::Camera& AmberEngine::LowRenderer::CameraController::GetCamera()
{
	return m_camera;
}
