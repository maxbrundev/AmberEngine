#include "Amberpch.h"

#include "AmberEngine/Maths/Transform.h"

#include <glm/gtc/matrix_transform.hpp>

AmberEngine::Maths::Transform::Transform(const glm::vec3 & p_position, const glm::vec3 & p_rotation, const glm::vec3 & p_scale)
{
	GenerateMatrices(p_position, p_rotation, p_scale);
}

void AmberEngine::Maths::Transform::GenerateMatrices(glm::vec3 p_position, glm::vec3 p_rotation, glm::vec3 p_scale)
{
	glm::mat4 positionMatrix{ 1.0f };
	glm::mat4 rotationMatrix{ 1.0f };
	glm::mat4 scaleMatrix   { 1.0f };

	glm::mat4 rotationX{ 1.0f };
	rotationX = glm::rotate(rotationX, glm::radians(p_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 rotationY{ 1.0f };
	rotationY = glm::rotate(rotationY, glm::radians(p_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 rotationZ{ 1.0f };
	rotationZ = glm::rotate(rotationZ, glm::radians(p_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	rotationMatrix = rotationX * rotationY * rotationZ;

	m_worldMatrix = glm::translate(positionMatrix, p_position) * rotationMatrix * glm::scale(scaleMatrix, p_scale);

	m_woldPosition	= p_position;
	m_worldRotation = p_rotation;
	m_worldScale	= p_scale;
}
