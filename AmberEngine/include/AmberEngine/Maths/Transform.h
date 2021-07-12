#pragma once

#include "AmberEngine/API/Export.h"

#include <glm/glm.hpp>

namespace AmberEngine::Maths
{
	class API_AMBERENGINE Transform
	{
	public:
		Transform(const glm::vec3& p_position = glm::vec3 { 0.0f, 0.0f, 0.0f }, 
				const glm::vec3& p_rotation   = glm::vec3 { 0.0f, 0.0f, 0.0f },
				const glm::vec3& p_scale      = glm::vec3 { 1.0f, 1.0f, 1.0f });

		~Transform() = default;

		glm::vec3& GetWorldPosition();
		glm::vec3& GetWorldRotation();
		glm::vec3& GetWorldScale();

		void SetTransform(const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale);

		void SetWorldPosition(const glm::vec3& p_pos);
		void SetWorldRotation(const glm::vec3& p_rot);
		void SetWorldScale(const glm::vec3& p_scale);

		void GenerateMatrices(glm::vec3 p_position, glm::vec3 p_rotation, glm::vec3 p_scale);

	private:
		glm::vec3 m_woldPosition;
		glm::vec3 m_worldRotation;
		glm::vec3 m_worldScale;

		glm::mat4 m_worldMatrix;
	};
}