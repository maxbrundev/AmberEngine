#pragma once

#include "AmberEngine/API/Export.h"

#include <glm/gtc/quaternion.hpp>

#include "AmberEngine/Maths/ETransformState.h"

namespace AmberEngine::Maths
{
	class API_AMBERENGINE Transform
	{
	public:
		Transform(const glm::vec3& p_position = glm::vec3 { 0.0f, 0.0f, 0.0f }, const glm::vec3& p_rotation = glm::vec3 { 0.0f, 0.0f, 0.0f },const glm::vec3& p_scale = glm::vec3 { 1.0f, 1.0f, 1.0f });
		~Transform();

		void GenerateMatricesWorld(glm::vec3 p_position, glm::vec3 p_rotation, glm::vec3 p_scale);
		void GenerateMatricesLocal(glm::vec3 p_position, glm::vec3 p_rotation, glm::vec3 p_scale);
		void UpdateWorldMatrix();
		void UpdateLocalMatrix();

		bool HasParent() const;

		void TranslateLocal(const glm::vec3& p_translation);
		void RotateLocal(const glm::vec3& p_rotation);
		void ScaleLocal(const glm::vec3& p_scale);

		void SetParent(Transform& p_parent);
		bool RemoveParent();
		void HandleParentTransformCallback(ETransformState p_state);

		uint64_t AddChildrenCallback(const std::function<void(ETransformState)>& p_callback);

		void SetLocalPosition(glm::vec3 p_newPosition);
		void SetLocalRotationEuler(glm::vec3 p_newRotation);
		void SetLocalRotation(glm::quat p_newRotation);
		void SetLocalScale(glm::vec3 p_newScale);

		void SetWorldPosition(glm::vec3 p_newPosition);
		void SetWorldRotationEuler(glm::vec3 p_newRotation);
		void SetWorldRotation(glm::quat p_newRotation);
		void SetWorldScale(glm::vec3 p_newScale);

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

		uint64_t m_childCallbackID  = 0;
		uint64_t m_parentCallbackID = 0;

		std::unordered_map<uint64_t, std::function<void(ETransformState)>> m_childrenTransformCallbacks;
	};
}
