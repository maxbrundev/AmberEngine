#pragma once

#include "AmberEngine/API/Export.h"

#include <glm/gtc/quaternion.hpp>

#include "AmberEngine/Maths/TransformNotifier.h"

namespace AmberEngine::Maths
{
	class API_AMBERENGINE Transform
	{
	public:
		Transform(const glm::vec3& p_localPosition = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& p_localRotation = glm::vec3(0.0f, 0.0f, 0.0f),const glm::vec3& p_localScale = glm::vec3(1.0f, 1.0f, 1.0f ));
		Transform(const Transform& p_other);

		Transform& operator=(const Transform& p_other);

		~Transform();

		void GenerateMatricesWorld(const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale);
		void GenerateMatricesLocal(const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale);
		void UpdateWorldMatrix();
		void UpdateLocalMatrix();

		bool HasParent() const;

		void TranslateLocal(const glm::vec3& p_translation);
		void RotateLocal(const glm::vec3& p_rotation);
		void ScaleLocal(const glm::vec3& p_scale);

		void SetParent(Transform& p_parent);
		bool RemoveParent();
		void NotificationHandler(TransformNotifier::ENotification p_notification);

		void SetLocalPosition(const glm::vec3& p_newPosition);
		void SetLocalRotationEuler(const glm::vec3& p_newRotation);
		void SetLocalRotation(const glm::quat& p_newRotation);
		void SetLocalScale(const glm::vec3& p_newScale);

		void SetWorldPosition(const glm::vec3& p_newPosition);
		void SetWorldRotationEuler(const glm::vec3& p_newRotation);
		void SetWorldRotation(const glm::quat& p_newRotation);
		void SetWorldScale(const glm::vec3& p_newScale);

		const glm::vec3& GetLocalPosition() const;
		const glm::vec3& GetLocalRotationEuler() const;
		const glm::quat& GetLocalRotation() const;
		const glm::vec3& GetLocalScale() const;

		const glm::vec3& GetWorldPosition() const;
		const glm::vec3& GetWorldRotationEuler() const;
		const glm::quat& GetWorldRotation() const;
		const glm::vec3& GetWorldScale() const;

		glm::vec3 GetWorldForward() const;
		glm::vec3 GetWorldUp() const;
		glm::vec3 GetWorldRight() const;

		glm::vec3 GetLocalForward() const;
		glm::vec3 GetLocalUp() const;
		glm::vec3 GetLocalRight() const;

		const glm::mat4& GetLocalMatrix() const;
		const glm::mat4& GetWorldMatrix() const;

	private:
		void PreDecomposeWorldMatrix();
		void PreDecomposeLocalMatrix();

	public:
		TransformNotifier Notifier;

	private:
		glm::vec3 m_localPosition;
		glm::vec3 m_localRotationEuler;
		glm::vec3 m_localScale;

		glm::vec3 m_worldPosition;
		glm::vec3 m_worldRotationEuler;
		glm::vec3 m_worldScale;

		glm::mat4 m_localMatrix;
		glm::mat4 m_worldMatrix;

		glm::quat m_worldRotation;
		glm::quat m_localRotation;

		Transform* m_parent;

		uint64_t m_notificationHandlerID;
	};
}
