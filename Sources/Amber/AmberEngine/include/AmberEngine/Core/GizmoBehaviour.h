#pragma once

#include "AmberEngine/Core/ECS/Actor.h"

namespace AmberEngine::Core
{
	enum class EGizmoOperation
	{
		TRANSLATE,
		ROTATE,
		SCALE
	};

	class GizmoBehaviour
	{
	public:
		enum class EDirection
		{
			X,
			Y,
			Z
		};
		
		
		bool IsSnappedBehaviourEnabled() const;

		void StartPicking(ECS::Actor& p_target, const glm::vec3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction);
		void StartPicking(Maths::Transform& p_target, const glm::vec3& p_cameraPosition, EGizmoOperation p_operation, EDirection p_direction);

		void StopPicking();

		void ApplyOperation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec3& p_cameraPosition, const glm::vec2& p_viewSize);

		void SetCurrentMouse(const glm::vec2& p_mousePosition);

		bool IsPicking() const;

		EDirection GetDirection() const;

		glm::vec3 GetMouseRay(const glm::vec2& p_mousePos, const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize);

	private:
		glm::vec3 GetFakeDirection() const;

		glm::vec3 GetRealDirection(bool p_relative = false) const;

		glm::vec2 GetScreenDirection(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const;
		
		void ApplyTranslation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec3& p_cameraPosition, const glm::vec2& p_viewSize);

		
		void ApplyRotation(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const;

		void ApplyScale(const glm::mat4& p_viewMatrix, const glm::mat4& p_projectionMatrix, const glm::vec2& p_viewSize) const;

	private:
		bool m_firstMouse = true;
		bool m_firstPick = true;
		float m_distanceToActor = 0.0f;
		ECS::Actor* m_target = nullptr;
		Maths::Transform* m_targetTransform = nullptr;
		EGizmoOperation m_currentOperation;
		EDirection m_direction;
		Maths::Transform m_originalTransform;
		glm::vec3 m_initialOffset;
		glm::vec2 m_originMouse;
		glm::vec2 m_currentMouse;
		glm::vec2 m_screenDirection;
	};
}
