#include "Amberpch.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberCore/ECS/Components/CModelRenderer.h"
#include "AmberCore/ECS/Components/CPhysicalBox.h"
#include "AmberCore/ECS/Components/CPhysicalCapsule.h"
#include "AmberCore/ECS/Components/CPhysicalSphere.h"

#include "AmberEditor/Core/CameraController.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/quaternion.hpp>

#include "AmberTools/Global/ServiceLocator.h"

#include "AmberEditor/Core/EditorAction.h"

AmberEditor::Core::CameraController::CameraController(AmberRendering::Entities::Camera& p_camera, glm::vec3& p_position, glm::quat& p_rotation, bool p_enableFocusInputs) :
	m_window(AmberTools::Global::ServiceLocator::Get<AmberWindowing::Context::Window>()),
	m_inputManager(AmberTools::Global::ServiceLocator::Get<AmberWindowing::Inputs::InputManager>()),
	m_camera(p_camera),
	m_position(p_position),
	m_rotation(p_rotation),
	m_enableFocusInputs(p_enableFocusInputs),
	m_targetPosition{},
	m_currentMovement{}
{
}

float GetActorFocusDist(AmberCore::ECS::Actor& p_actor)
{
	float distance = 4.0f;

	if (p_actor.IsActive())
	{
		if (auto physicalBox = p_actor.GetComponent<AmberCore::ECS::Components::CPhysicalBox>())
		{
			distance = std::max(distance, std::max
			(
				std::max
				(
					physicalBox->GetSize().x * p_actor.transform.GetWorldScale().x,
					physicalBox->GetSize().y * p_actor.transform.GetWorldScale().y
				),
				physicalBox->GetSize().z * p_actor.transform.GetWorldScale().z
			) * 1.5f);
		}

		if (auto physicalSphere = p_actor.GetComponent<AmberCore::ECS::Components::CPhysicalSphere>())
		{
			distance = std::max(distance, std::max
			(
				std::max
				(
					physicalSphere->GetRadius() * p_actor.transform.GetWorldScale().x,
					physicalSphere->GetRadius() * p_actor.transform.GetWorldScale().y
				),
				physicalSphere->GetRadius() * p_actor.transform.GetWorldScale().z
			) * 1.5f);
		}

		if (auto physicalCapsule = p_actor.GetComponent<AmberCore::ECS::Components::CPhysicalCapsule>())
		{
			distance = std::max(distance, std::max
			(
				std::max
				(
					physicalCapsule->GetRadius() * p_actor.transform.GetWorldScale().x,
					physicalCapsule->GetHeight() * p_actor.transform.GetWorldScale().y
				),
				physicalCapsule->GetRadius() * p_actor.transform.GetWorldScale().z
			) * 1.5f);
		}

		if (auto modelRenderer = p_actor.GetComponent<AmberCore::ECS::Components::CModelRenderer>())
		{
			if (auto model = modelRenderer->GetModel())
			{
				const auto& boundingSphere = model->GetBoundingSphere();
				const auto& actorScale = p_actor.transform.GetWorldScale();
				const float scaleFactor = std::max(std::max(actorScale.x, actorScale.y), actorScale.z);

				distance = std::max(distance, (boundingSphere.Radius + glm::length(boundingSphere.Position)) * scaleFactor * 2.0f);
			}
			else
			{
				distance = std::max(distance, 10.0f);
			}
		}

		for (auto child : p_actor.GetChildren())
		{
			distance = std::max(distance, GetActorFocusDist(*child));
		}
	}

	return distance;
}

void AmberEditor::Core::CameraController::Update(float p_deltaTime)
{
	if (m_enableFocusInputs && !ImGui::IsAnyItemActive())
	{
		HandleFocusInputs();
	}

	if (!m_cameraDestinations.empty())
	{
		HandleCameraDestinations(p_deltaTime);
	}
	else
	{
		HandleInputs(p_deltaTime);
		HandleMouse();
		HandleCameraZoom();
	}

	if (m_inputManager.IsMouseButtonPressed(AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		m_rightMousePressed = true;
		m_window.SetCursorMode(AmberWindowing::Context::ECursorMode::DISABLED);
	}

	if (m_inputManager.IsMouseButtonReleased(AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_RIGHT))
	{
		m_isFirstMouse = true;
		m_rightMousePressed = false;
		m_window.SetCursorMode(AmberWindowing::Context::ECursorMode::NORMAL);
	}

	if (m_inputManager.IsMouseButtonPressed(AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_3))
	{
		m_middleMousePressed = true;
		m_window.SetCursorMode(AmberWindowing::Context::ECursorMode::DISABLED);
	}

	if (m_inputManager.IsMouseButtonReleased(AmberWindowing::Inputs::EMouseButton::MOUSE_BUTTON_3))
	{
		m_isFirstMiddleMouse = true;
		m_middleMousePressed = false;
		m_window.SetCursorMode(AmberWindowing::Context::ECursorMode::NORMAL);
	}

	auto& io = ImGui::GetIO();
	io.DisableMouseUpdate = m_rightMousePressed || m_middleMousePressed;
}

void AmberEditor::Core::CameraController::SetPosition(const glm::vec3& p_position) const
{
	m_position = p_position;
}

void AmberEditor::Core::CameraController::SetPosition(float p_posX, float p_posY, float p_posZ) const
{
	m_position.x = p_posX;
	m_position.y = p_posY;
	m_position.z = p_posZ;
}

void AmberEditor::Core::CameraController::SetRotation(const glm::quat& p_rotation) const
{
	m_rotation = p_rotation;
}

const glm::quat& AmberEditor::Core::CameraController::GetRotation() const
{
	return m_rotation;
}

void AmberEditor::Core::CameraController::SetSpeed(float p_value)
{
	m_moveSpeed = p_value;
}

float AmberEditor::Core::CameraController::GetSpeed() const
{
	return m_moveSpeed;
}

const glm::vec3& AmberEditor::Core::CameraController::GetPosition() const
{
	return m_position;
}

bool AmberEditor::Core::CameraController::IsRightMousePressed() const
{
	return m_rightMousePressed;
}

void AmberEditor::Core::CameraController::MoveToTarget(AmberCore::ECS::Actor& p_target)
{
	m_cameraDestinations.push({ p_target.transform.GetWorldPosition() - m_rotation * glm::vec3(0.0f, 0.0f, 1.0f) * GetActorFocusDist(p_target), m_rotation });
}

void AmberEditor::Core::CameraController::HandleFocusInputs()
{
	if (EDITOR_EXEC(IsAnyActorSelected()))
	{
		auto& selectedActor = EDITOR_EXEC(GetSelectedActor());
		const glm::vec3 targetPosition = selectedActor.transform.GetWorldPosition();
		const float distance = GetActorFocusDist(selectedActor);

		auto orbit = [this, &targetPosition, distance](const glm::vec3& p_offsetDirection, const glm::vec3& p_up)
		{
			m_cameraDestinations.push({ targetPosition + p_offsetDirection * distance, glm::quatLookAt(p_offsetDirection, p_up) });
		};

		if (m_inputManager.IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_F))
			MoveToTarget(selectedActor);

		if (m_inputManager.IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_UP))
			orbit(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		if (m_inputManager.IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_DOWN))
			orbit(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		if (m_inputManager.IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_LEFT))
			orbit(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		if (m_inputManager.IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_RIGHT))
			orbit(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		if (m_inputManager.IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_PAGE_DOWN))
			orbit(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		if (m_inputManager.IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_PAGE_UP))
			orbit(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void AmberEditor::Core::CameraController::HandleCameraDestinations(float p_deltaTime)
{
	m_targetPosition = glm::vec3(0.0f);
	m_currentMovement = glm::vec3(0.0f);

	while (m_cameraDestinations.size() != 1)
	{
		m_cameraDestinations.pop();
	}

	auto& [destPosition, destRotation] = m_cameraDestinations.front();

	const float t = m_focusLerpCoefficient * p_deltaTime;

	if (glm::distance(m_position, destPosition) <= 0.03f)
	{
		m_position = destPosition;
		m_rotation = destRotation;
		m_isFirstMouse = true;

		m_cameraDestinations.pop();
	}
	else
	{
		m_position = glm::lerp(m_position, destPosition, t);
		m_rotation = glm::slerp(m_rotation, destRotation, t);
	}
}

void AmberEditor::Core::CameraController::HandleInputs(float p_deltaTime)
{
	m_targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	if (m_rightMousePressed) 
	{
		bool run = m_inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_LEFT_SHIFT) == AmberWindowing::Inputs::EKeyState::KEY_DOWN;

		float velocity = m_moveSpeed * p_deltaTime * (run ? 2.0f : 1.0f);

		if (m_inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_W) == AmberWindowing::Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_rotation * glm::vec3(0.0f, 0.0f, 1.0f) * velocity;
		}

		if (m_inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_A) == AmberWindowing::Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_rotation * glm::vec3(1.0f, 0.0f, 0.0f) * velocity;
		}

		if (m_inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_S) == AmberWindowing::Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_rotation * glm::vec3(0.0f, 0.0f, 1.0f) * -velocity;
		}

		if (m_inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_D) == AmberWindowing::Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += m_rotation * glm::vec3(1.0f, 0.0f, 0.0f) * -velocity;
		}

		if (m_inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_E) == AmberWindowing::Inputs::EKeyState::KEY_DOWN)
		{
			m_targetPosition += glm::vec3(0.0f, velocity, 0.0f);
		}

		if (m_inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_Q) == AmberWindowing::Inputs::EKeyState::KEY_DOWN)
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

void AmberEditor::Core::CameraController::HandleMouse()
{
	if(m_rightMousePressed || m_middleMousePressed)
	{
		const auto[xPos, yPos] = m_inputManager.GetMousePosition();

		if (m_isFirstMouse)
		{
			m_lastMousePosX = xPos;
			m_lastMousePosY = yPos;

			m_eulerRotation = glm::degrees(glm::eulerAngles(m_rotation));

			m_eulerRotation = RemoveRoll(m_eulerRotation);

			m_isFirstMouse = false;
		}

		if (m_isFirstMiddleMouse)
		{
			m_lastMousePosX = xPos;
			m_lastMousePosY = yPos;

			m_isFirstMiddleMouse = false;
		}

		const glm::vec2 mouseOffset
		{
			static_cast<float>(xPos - m_lastMousePosX),
			static_cast<float>(m_lastMousePosY - yPos)
		};

		m_lastMousePosX = xPos;
		m_lastMousePosY = yPos;

		if (m_rightMousePressed)
		{
			auto mouseDelta = mouseOffset * m_mouseSensitivity;

			m_eulerRotation.y -= mouseDelta.x;
			m_eulerRotation.x -= mouseDelta.y;
			m_eulerRotation.x = std::max(std::min(m_eulerRotation.x, 90.0f), -90.0f);

			m_rotation = glm::qua(glm::radians(m_eulerRotation));
		}

		if (m_middleMousePressed)
		{
			auto mouseDelta = mouseOffset * 0.01f;

			const glm::vec3 right = m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
			const glm::vec3 up = m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);

			const glm::vec3 delta = right * mouseDelta.x + -up * mouseDelta.y;

			m_position += delta;
		}
	}
}

void AmberEditor::Core::CameraController::HandleCameraZoom() const
{
	m_position += m_rotation * glm::vec3(0.0f, 0.0f, 1.0f) * ImGui::GetIO().MouseWheel;
}
