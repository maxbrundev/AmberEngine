#pragma once

#include "AmberEngine/API/Export.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace AmberEngine::Maths
{
	class API_AMBERENGINE Transform
	{
	public:
		Transform(const glm::vec3& p_position = glm::vec3 { 0.0f, 0.0f, 0.0f }, const glm::vec3& p_rotation = glm::vec3 { 0.0f, 0.0f, 0.0f },const glm::vec3& p_scale = glm::vec3 { 1.0f, 1.0f, 1.0f });
		~Transform() = default;

		void GenerateMatrices(glm::vec3 p_position, glm::vec3 p_rotation, glm::vec3 p_scale);
		void UpdateWorldMatrix();

		bool HasParent() const;

		void SetLocalPosition(glm::vec3 p_newPosition);
		void SetLocalRotation(glm::vec3 p_newRotation);
		void SetLocalScale(glm::vec3 p_newScale);

		glm::vec3 GetWorldForward() const;

		void TranslateLocal(const glm::vec3& p_translation);
		void RotateLocal(const glm::vec3& p_rotation);
		void ScaleLocal(const glm::vec3& p_scale);

		glm::mat4& GetLocalMatrix();
		glm::mat4& GetWorldMatrix();

	private:
		void PreDecomposeWorldMatrix();

	public:
		glm::vec3 m_localPosition;
		glm::vec3 m_localRotation;
		glm::vec3 m_localScale;

		glm::vec3 m_worldPosition;
		glm::vec3 m_worldRotation;
		glm::vec3 m_worldScale;

		glm::mat4 m_localMatrix;
		glm::mat4 m_worldMatrix;

		glm::quat m_worldRotationQuat;
		glm::quat m_localRotationQuat;

		Transform* m_parent;
	};
}