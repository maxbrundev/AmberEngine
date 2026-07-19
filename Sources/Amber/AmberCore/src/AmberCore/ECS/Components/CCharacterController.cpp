#include "Amberpch.h"

#include "AmberCore/ECS/Components/CCharacterController.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberCore/ECS/Components/CCamera.h"
#include "AmberCore/ECS/PhysicsWrapper.h"
#include "AmberCore/Helpers/GUIDrawer.h"
#include "AmberCore/Helpers/Serializer.h"
#include "AmberDebug/Logger.h"
#include "AmberTools/Global/ServiceLocator.h"
#include "AmberWindowing/Context/Window.h"
#include "AmberWindowing/Inputs/InputManager.h"

AmberCore::ECS::Components::CCharacterController::CCharacterController(Actor& p_owner) : CPhysicalCapsule(p_owner)
{
}

std::string AmberCore::ECS::Components::CCharacterController::GetName()
{
	return "Character Controller";
}

void AmberCore::ECS::Components::CCharacterController::SetMouseSensitivity(float p_sensitivity)
{
	m_mouseSensitivity = p_sensitivity;
}

void AmberCore::ECS::Components::CCharacterController::SetFovScale(float p_scale)
{
	m_fovScale = p_scale;
}

float AmberCore::ECS::Components::CCharacterController::GetMouseSensitivity() const
{
	return m_mouseSensitivity;
}

float AmberCore::ECS::Components::CCharacterController::GetFovScale() const
{
	return m_fovScale;
}

bool AmberCore::ECS::Components::CCharacterController::IsGrounded() const
{
	return m_isGrounded;
}

bool AmberCore::ECS::Components::CCharacterController::IsRunning() const
{
	return m_running;
}

bool AmberCore::ECS::Components::CCharacterController::IsCrouching() const
{
	return m_crouching;
}

void AmberCore::ECS::Components::CCharacterController::OnStart()
{
	FetchReferences();

	m_defaultHeight = GetHeight();
	SetAngularFactor(glm::vec3(0.0f, 0.0f, 0.0f));
	SetAngularVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

	m_currentEye = m_eyeHeight;
	m_currentFov = m_baseFov;

	if (m_mouseLocked)
	{
		AmberTools::Global::ServiceLocator::Get<AmberWindowing::Context::Window>().SetCursorMode(AmberWindowing::Context::ECursorMode::DISABLED);
	}
}

void AmberCore::ECS::Components::CCharacterController::OnUpdate(float p_deltaTime)
{
	if (!FetchReferences())
	{
		if (!m_missingReferencesWarned)
		{
			AB_LOG_WARNING("Character Controller on \"" + Owner.GetName() + "\" requires a child actor with a Camera component");

			m_missingReferencesWarned = true;
		}

		return;
	}

	auto& inputManager = AmberTools::Global::ServiceLocator::Get<AmberWindowing::Inputs::InputManager>();

	if (inputManager.IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_LEFT_ALT))
	{
		m_mouseLocked = !m_mouseLocked;
		m_firstMouse = true;

		auto& window = AmberTools::Global::ServiceLocator::Get<AmberWindowing::Context::Window>();

		if (m_mouseLocked)
		{
			window.SetCursorMode(AmberWindowing::Context::ECursorMode::DISABLED);
		}
		else
		{
			window.SetCursorMode(AmberWindowing::Context::ECursorMode::NORMAL);
			glm::vec3 velocity = GetLinearVelocity();
			SetLinearVelocity(glm::vec3(0.0f, velocity.y, 0.0f));
		}
	}

	m_jumpBuffer -= p_deltaTime;

	CheckGround();

	if (m_isGrounded)
	{
		m_timeSinceGrounded = 0.0f;
	}
	else
	{
		m_timeSinceGrounded += p_deltaTime;
	}

	if (m_mouseLocked)
	{
		HandleRotation(p_deltaTime);
		HandleMovement(p_deltaTime);
		UpdateCamera(p_deltaTime);
	}
}

AmberCore::ECS::Actor* AmberCore::ECS::Components::CCharacterController::FindCameraChild()
{
	for (Actor* child : Owner.GetChildren())
	{
		if (child->GetComponent<CCamera>())
		{
			return child;
		}
	}

	return nullptr;
}

bool AmberCore::ECS::Components::CCharacterController::FetchReferences()
{
	if (!m_camera)
	{
		if (Actor* cameraActor = FindCameraChild())
		{
			m_cameraActor = cameraActor;
			m_camera = cameraActor->GetComponent<CCamera>();
		}
	}

	return m_camera != nullptr;
}

void AmberCore::ECS::Components::CCharacterController::HandleMovement(float p_deltaTime)
{
	auto& inputManager = AmberTools::Global::ServiceLocator::Get<AmberWindowing::Inputs::InputManager>();

	glm::vec3 velocity = GetLinearVelocity();
	glm::vec3 horizontalVelocity = glm::vec3(velocity.x, 0.0f, velocity.z);

	glm::vec3 inputDirection = glm::vec3(0.0f, 0.0f, 0.0f);

	if (inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_W) == AmberWindowing::Inputs::EKeyState::KEY_DOWN)
	{
		inputDirection += Owner.transform.GetWorldForward();
	}

	if (inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_S) == AmberWindowing::Inputs::EKeyState::KEY_DOWN)
	{
		inputDirection -= Owner.transform.GetWorldForward();
	}

	if (inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_A) == AmberWindowing::Inputs::EKeyState::KEY_DOWN)
	{
		inputDirection += Owner.transform.GetWorldRight();
	}

	if (inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_D) == AmberWindowing::Inputs::EKeyState::KEY_DOWN)
	{
		inputDirection -= Owner.transform.GetWorldRight();
	}

	inputDirection.y = 0.0f;

	const bool hasInputDirection = inputDirection != glm::vec3(0.0f, 0.0f, 0.0f);

	if (hasInputDirection)
	{
		inputDirection = glm::normalize(inputDirection);
	}

	if (inputManager.IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_SPACE))
	{
		m_jumpBuffer = m_jumpBufferTime;
	}

	m_running = inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_LEFT_SHIFT) == AmberWindowing::Inputs::EKeyState::KEY_DOWN;

	if (m_running && hasInputDirection)
	{
		glm::vec3 forward = Owner.transform.GetWorldForward();
		forward.y = 0.0f;
		m_running = glm::dot(inputDirection, glm::normalize(forward)) > 0.5f;
	}
	else
	{
		m_running = false;
	}

	bool wantsCrouch = inputManager.GetKey(AmberWindowing::Inputs::EKey::KEY_LEFT_CONTROL) == AmberWindowing::Inputs::EKeyState::KEY_DOWN;

	if (!m_crouching && wantsCrouch && m_isGrounded)
	{
		StartCrouch();
	}
	else if (m_crouching && !wantsCrouch)
	{
		TryStandUp();
	}

	float maxSpeed = m_walkSpeed;

	if (m_crouching)
	{
		maxSpeed = m_crouchSpeed;
	}
	else if (m_running)
	{
		maxSpeed = m_runSpeed;
	}

	if (m_isGrounded)
	{
		if (hasInputDirection)
		{
			horizontalVelocity += inputDirection * (m_groundAcceleration * p_deltaTime);
			float length = glm::length(horizontalVelocity);

			if (length > maxSpeed)
			{
				float newLength = glm::mix(length, maxSpeed, std::min(1.0f, m_groundFriction * p_deltaTime));
				horizontalVelocity = glm::normalize(horizontalVelocity) * newLength;
			}
		}
		else
		{
			horizontalVelocity = horizontalVelocity * std::max(0.0f, 1.0f - m_groundFriction * p_deltaTime);
		}
	}
	else
	{
		if (hasInputDirection)
		{
			float previousLength = glm::length(horizontalVelocity);
			horizontalVelocity += inputDirection * (m_airAcceleration * p_deltaTime);
			float length = glm::length(horizontalVelocity);
			float maxAirSpeed = std::max(previousLength, maxSpeed);

			if (length > maxAirSpeed)
			{
				horizontalVelocity = glm::normalize(horizontalVelocity) * maxAirSpeed;
			}
		}
	}

	if (m_jumpBuffer > 0.0f && (m_isGrounded || m_timeSinceGrounded < m_coyoteTime))
	{
		m_jumpBuffer = 0.0f;
		m_timeSinceGrounded = m_coyoteTime + 1.0f;
		velocity.y = m_jumpForce;
	}

	SetLinearVelocity(glm::vec3(horizontalVelocity.x, velocity.y, horizontalVelocity.z));
}

void AmberCore::ECS::Components::CCharacterController::StartCrouch()
{
	m_crouching = true;
	SetHeight(m_crouchCapsuleHeight);
}

void AmberCore::ECS::Components::CCharacterController::TryStandUp()
{
	if (!m_crouching)
	{
		return;
	}

	if (CanStandUp())
	{
		m_crouching = false;
		SetHeight(m_defaultHeight);
	}
}

bool AmberCore::ECS::Components::CCharacterController::CanStandUp()
{
	glm::vec3 start = Owner.transform.GetWorldPosition();
	float distance = m_defaultHeight * 0.5f + GetRadius() + 0.05f;

	return !RayHitsSolid(start, glm::vec3(0.0f, 1.0f, 0.0f), distance);
}

void AmberCore::ECS::Components::CCharacterController::UpdateCamera(float p_deltaTime)
{
	float targetEye = m_eyeHeight;

	if (m_crouching)
	{
		targetEye = m_crouchEyeHeight;
	}

	m_currentEye = glm::mix(m_currentEye, targetEye, std::min(1.0f, m_eyeSmoothing * p_deltaTime));

	m_cameraActor->transform.SetLocalPosition(glm::vec3(0.0f, m_currentEye, 0.0f));

	glm::vec3 velocity = GetLinearVelocity();

	float speed = glm::length(glm::vec3(velocity.x, 0.0f, velocity.z));

	float targetFov = m_baseFov;

	if (m_running && speed > m_walkSpeed + 0.5f)
	{
		targetFov = m_baseFov + m_sprintFovKick;
	}

	m_currentFov = glm::mix(m_currentFov, targetFov, std::min(1.0f, m_fovSmoothing * p_deltaTime));
	m_camera->SetFov(m_currentFov * m_fovScale);
}

void AmberCore::ECS::Components::CCharacterController::HandleRotation(float p_deltaTime)
{
	auto& inputManager = AmberTools::Global::ServiceLocator::Get<AmberWindowing::Inputs::InputManager>();

	auto [mouseX, mouseY] = inputManager.GetMousePosition();

	glm::vec2 mousePosition = glm::vec2(static_cast<float>(mouseX), static_cast<float>(mouseY));

	if (m_firstMouse)
	{
		m_previousMouse = mousePosition;
		m_firstMouse = false;
	}

	glm::vec2 mouseOffset = glm::vec2(mousePosition.x - m_previousMouse.x, m_previousMouse.y - mousePosition.y);

	m_previousMouse = mousePosition;

	m_mouseLook += mouseOffset * m_mouseSensitivity;

	if (m_mouseLook.y > 89.0f)
	{
		m_mouseLook.y = 89.0f;
	}

	if (m_mouseLook.y < -89.0f)
	{
		m_mouseLook.y = -89.0f;
	}

	m_cameraActor->transform.SetLocalRotation(glm::quat(glm::radians(glm::vec3(-m_mouseLook.y, 0.0f, 0.0f))));
	Owner.transform.SetLocalRotation(glm::quat(glm::radians(glm::vec3(0.0f, -m_mouseLook.x, 0.0f))));
}

bool AmberCore::ECS::Components::CCharacterController::RayHitsSolid(const glm::vec3& p_start, const glm::vec3& p_direction, float p_distance)
{
	AmberCore::ECS::PhysicsWrapper::RaycastHit hit;

	if (AmberCore::ECS::PhysicsWrapper::Raycast(p_start, p_direction, p_distance, hit))
	{
		if (hit.FirstResultObject && &hit.FirstResultObject->Owner != &Owner && !hit.FirstResultObject->IsTrigger())
		{
			return true;
		}

		for (CPhysicalObject* resultObject : hit.ResultObjects)
		{
			if (&resultObject->Owner != &Owner && !resultObject->IsTrigger())
			{
				return true;
			}
		}
	}

	return false;
}

bool AmberCore::ECS::Components::CCharacterController::IsGroundedAtOffset(const glm::vec3& p_offset)
{
	float halfHeight = GetHeight() * 0.5f + GetRadius();
	glm::vec3 start = Owner.transform.GetWorldPosition() + p_offset - glm::vec3(0.0f, halfHeight - 0.1f, 0.0f);

	return RayHitsSolid(start, glm::vec3(0.0f, -1.0f, 0.0f), 0.25f);
}

void AmberCore::ECS::Components::CCharacterController::CheckGround()
{
	float radius = GetRadius() * 0.9f;

	m_isGrounded =
		IsGroundedAtOffset(glm::vec3(0.0f, 0.0f, 0.0f)) ||
		IsGroundedAtOffset(glm::vec3(-radius, 0.0f, 0.0f)) ||
		IsGroundedAtOffset(glm::vec3(radius, 0.0f, 0.0f)) ||
		IsGroundedAtOffset(glm::vec3(0.0f, 0.0f, radius)) ||
		IsGroundedAtOffset(glm::vec3(0.0f, 0.0f, -radius));
}

void AmberCore::ECS::Components::CCharacterController::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CPhysicalCapsule::OnSerialize(p_doc, p_node);

	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "walk_speed", m_walkSpeed);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "run_speed", m_runSpeed);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "ground_acceleration", m_groundAcceleration);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "air_acceleration", m_airAcceleration);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "ground_friction", m_groundFriction);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "jump_force", m_jumpForce);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "coyote_time", m_coyoteTime);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "jump_buffer_time", m_jumpBufferTime);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "crouch_speed", m_crouchSpeed);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "crouch_capsule_height", m_crouchCapsuleHeight);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "crouch_eye_height", m_crouchEyeHeight);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "mouse_sensitivity", m_mouseSensitivity);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "base_fov", m_baseFov);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "sprint_fov_kick", m_sprintFovKick);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "eye_height", m_eyeHeight);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "eye_smoothing", m_eyeSmoothing);
	AmberCore::Helpers::Serializer::SerializeFloat(p_doc, p_node, "fov_smoothing", m_fovSmoothing);
}

void AmberCore::ECS::Components::CCharacterController::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	CPhysicalCapsule::OnDeserialize(p_doc, p_node);

	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "walk_speed", m_walkSpeed);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "run_speed", m_runSpeed);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "ground_acceleration", m_groundAcceleration);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "air_acceleration", m_airAcceleration);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "ground_friction", m_groundFriction);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "jump_force", m_jumpForce);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "coyote_time", m_coyoteTime);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "jump_buffer_time", m_jumpBufferTime);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "crouch_speed", m_crouchSpeed);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "crouch_capsule_height", m_crouchCapsuleHeight);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "crouch_eye_height", m_crouchEyeHeight);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "mouse_sensitivity", m_mouseSensitivity);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "base_fov", m_baseFov);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "sprint_fov_kick", m_sprintFovKick);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "eye_height", m_eyeHeight);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "eye_smoothing", m_eyeSmoothing);
	AmberCore::Helpers::Serializer::DeserializeFloat(p_doc, p_node, "fov_smoothing", m_fovSmoothing);
}

void AmberCore::ECS::Components::CCharacterController::OnInspector(AmberUI::WidgetContainer& p_root)
{
	AmberCore::Helpers::GUIDrawer::CreateSectionTitle(p_root, "Capsule");
	CPhysicalCapsule::OnInspector(p_root);

	AmberCore::Helpers::GUIDrawer::CreateSectionTitle(p_root, "Speeds");
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Walk Speed", m_walkSpeed, 0.1f, 0.0f, 100.0f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Run Speed", m_runSpeed, 0.1f, 0.0f, 100.0f);

	AmberCore::Helpers::GUIDrawer::CreateSectionTitle(p_root, "Acceleration");
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Ground Acceleration", m_groundAcceleration, 0.1f, 0.0f, 1000.0f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Air Acceleration", m_airAcceleration, 0.1f, 0.0f, 1000.0f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Ground Friction", m_groundFriction, 0.1f, 0.0f, 100.0f);

	AmberCore::Helpers::GUIDrawer::CreateSectionTitle(p_root, "Jump");
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Jump Force", m_jumpForce, 0.1f, 0.0f, 100.0f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Coyote Time", m_coyoteTime, 0.01f, 0.0f, 5.0f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Jump Buffer Time", m_jumpBufferTime, 0.01f, 0.0f, 5.0f);

	AmberCore::Helpers::GUIDrawer::CreateSectionTitle(p_root, "Crouch");
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Crouch Speed", m_crouchSpeed, 0.1f, 0.0f, 100.0f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Crouch Capsule Height", m_crouchCapsuleHeight, 0.01f, 0.01f, 10.0f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Crouch Eye Height", m_crouchEyeHeight, 0.01f, 0.0f, 10.0f);

	AmberCore::Helpers::GUIDrawer::CreateSectionTitle(p_root, "Camera");
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Mouse Sensitivity", m_mouseSensitivity, 0.001f, 0.0f, 10.0f);

	auto baseFovGatherer = [this]()
	{
		return m_baseFov;
	};

	auto baseFovProvider = [this](float p_value)
	{
		m_baseFov = p_value;
		m_currentFov = p_value;

		if (Actor* cameraActor = FindCameraChild())
		{
			cameraActor->GetComponent<CCamera>()->SetFov(m_baseFov * m_fovScale);
		}
	};

	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Base Fov", baseFovGatherer, baseFovProvider, 0.1f, 1.0f, 179.0f);

	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Sprint Fov Kick", m_sprintFovKick, 0.1f, 0.0f, 90.0f);

	auto eyeHeightGatherer = [this]()
	{
		return m_eyeHeight;
	};

	auto eyeHeightProvider = [this](float p_value)
	{
		m_eyeHeight = p_value;

		if (Actor* cameraActor = FindCameraChild())
		{
			cameraActor->transform.SetLocalPosition(glm::vec3(0.0f, m_eyeHeight, 0.0f));
		}
	};

	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Eye Height", eyeHeightGatherer, eyeHeightProvider, 0.01f, 0.0f, 10.0f);

	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Eye Smoothing", m_eyeSmoothing, 0.1f, 0.0f, 100.0f);
	AmberCore::Helpers::GUIDrawer::DrawScalar<float>(p_root, "Fov Smoothing", m_fovSmoothing, 0.1f, 0.0f, 100.0f);
}
