#pragma once

#include <glm/glm.hpp>

#include "AmberCore/ECS/Components/CPhysicalCapsule.h"

namespace AmberCore::ECS { class Actor; }

namespace AmberCore::ECS::Components
{
	class CCamera;

	class CCharacterController : public CPhysicalCapsule
	{
	public:
		CCharacterController(Actor& p_owner);

		std::string GetName() override;

		void SetMouseSensitivity(float p_sensitivity);
		void SetFovScale(float p_scale);

		float GetMouseSensitivity() const;
		float GetFovScale() const;

		bool IsGrounded() const;
		bool IsRunning() const;
		bool IsCrouching() const;

		virtual void OnStart() override;
		virtual void OnUpdate(float p_deltaTime) override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;

	private:
		Actor* FindCameraChild();
		bool FetchReferences();
		void CheckGround();
		void HandleRotation(float p_deltaTime);
		void HandleMovement(float p_deltaTime);
		void UpdateCamera(float p_deltaTime);
		void StartCrouch();
		void TryStandUp();
		bool CanStandUp();
		bool RayHitsSolid(const glm::vec3& p_start, const glm::vec3& p_direction, float p_distance);
		bool IsGroundedAtOffset(const glm::vec3& p_offset);

	private:
		float m_walkSpeed = 5.0f;
		float m_runSpeed = 8.5f;

		float m_groundAcceleration = 45.0f;
		float m_airAcceleration = 16.0f;
		float m_groundFriction = 10.0f;

		float m_jumpForce = 7.5f;
		float m_coyoteTime = 0.12f;
		float m_jumpBufferTime = 0.12f;

		float m_crouchSpeed = 2.5f;
		float m_crouchCapsuleHeight = 0.5f;
		float m_crouchEyeHeight = 0.3f;

		float m_mouseSensitivity = 0.05f;
		float m_baseFov = 60.0f;
		float m_sprintFovKick = 5.0f;
		float m_eyeHeight = 0.7f;
		float m_eyeSmoothing = 12.0f;
		float m_fovSmoothing = 8.0f;

		Actor* m_cameraActor = nullptr;
		CCamera* m_camera = nullptr;
		float m_defaultHeight = 1.0f;
		bool m_missingReferencesWarned = false;
		glm::vec2 m_mouseLook = glm::vec2(180.0f, 0.0f);
		glm::vec2 m_previousMouse = glm::vec2(0.0f, 0.0f);
		bool m_firstMouse = true;
		bool m_mouseLocked = true;
		bool m_isGrounded = false;
		float m_timeSinceGrounded = 0.0f;
		float m_jumpBuffer = 0.0f;
		bool m_crouching = false;
		float m_currentEye = 0.7f;
		float m_currentFov = 60.0f;
		bool m_running = false;
		float m_fovScale = 1.0f;
	};
}
