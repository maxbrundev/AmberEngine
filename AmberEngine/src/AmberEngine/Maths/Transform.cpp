#include "Amberpch.h"

#include "AmberEngine/Maths/Transform.h"

AmberEngine::Maths::Transform::Transform(const glm::vec3 & p_position, const glm::vec3 & p_rotation, const glm::vec3 & p_scale) : m_parent(nullptr)
{
	GenerateMatrices(p_position, p_rotation, p_scale);
}

void AmberEngine::Maths::Transform::GenerateMatrices(glm::vec3 p_position, glm::vec3 p_rotation, glm::vec3 p_scale)
{
	glm::mat4 positionMatrix { 1.0f };
	glm::mat4 rotationMatrix { 1.0f };
	glm::mat4 scaleMatrix    { 1.0f };

	glm::mat4 rotationX{ 1.0f };
	rotationX = glm::rotate(rotationX, glm::radians(p_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 rotationY{ 1.0f };
	rotationY = glm::rotate(rotationY, glm::radians(p_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 rotationZ{ 1.0f };
	rotationZ = glm::rotate(rotationZ, glm::radians(p_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	rotationMatrix = rotationZ * rotationY * rotationX;

	//m_localMatrix = glm::translate(positionMatrix, p_position) * rotationMatrix * glm::scale(scaleMatrix, p_scale);
	m_localMatrix = glm::scale(scaleMatrix, p_scale) * rotationMatrix * glm::translate(positionMatrix, p_position);
	m_localPosition	= p_position;
	m_localRotation = p_rotation;
	m_localScale	= p_scale;

	UpdateWorldMatrix();
}

void AmberEngine::Maths::Transform::UpdateWorldMatrix()
{
	m_worldMatrix = HasParent() ? m_parent->m_worldMatrix * m_localMatrix : m_localMatrix;

	PreDecomposeWorldMatrix();
}

bool AmberEngine::Maths::Transform::HasParent()
{
	return m_parent != nullptr;
}

void AmberEngine::Maths::Transform::SetLocalPosition(glm::vec3 p_newPosition)
{
	GenerateMatrices(p_newPosition, m_localRotation, m_localScale);
}

void AmberEngine::Maths::Transform::SetLocalRotation(glm::vec3 p_newRotation)
{
	GenerateMatrices(m_localPosition, p_newRotation, m_localScale);
}

void AmberEngine::Maths::Transform::SetLocalScale(glm::vec3 p_newScale)
{
	GenerateMatrices(m_localPosition, m_localRotation, p_newScale);
}

void AmberEngine::Maths::Transform::TranslateLocal(const glm::vec3& p_translation)
{
	SetLocalPosition(m_localPosition + p_translation);
}

void AmberEngine::Maths::Transform::RotateLocal(const glm::vec3& p_rotation)
{
	SetLocalRotation(m_localRotation + p_rotation);
}

void AmberEngine::Maths::Transform::ScaleLocal(const glm::vec3& p_scale)
{
	SetLocalScale(m_localScale * p_scale);
}

glm::mat4 AmberEngine::Maths::Transform::GetLocalMatrix()
{
	return m_localMatrix;
}

glm::mat4 AmberEngine::Maths::Transform::GetWorldMatrix()
{
	return m_worldMatrix;
}

void AmberEngine::Maths::Transform::PreDecomposeWorldMatrix()
{
	m_worldPosition.x = m_worldMatrix[3][0];
	m_worldPosition.y = m_worldMatrix[3][1];
	m_worldPosition.z = m_worldMatrix[3][2];

	//m_worldScale.x = glm::sqrt(m_worldMatrix[0][0] * m_worldMatrix[0][0] + m_worldMatrix[0][1] * m_worldMatrix[0][1] + m_worldMatrix[0][2] * m_worldMatrix[0][2]);
	glm::vec3 scaleX(m_worldMatrix[0][0], m_worldMatrix[0][1], m_worldMatrix[0][2]);
	glm::vec3 scaleY(m_worldMatrix[1][0], m_worldMatrix[1][1], m_worldMatrix[1][2]);
	glm::vec3 scaleZ(m_worldMatrix[2][0], m_worldMatrix[2][1], m_worldMatrix[2][2]);


	m_worldScale.x = glm::length(scaleX);
	m_worldScale.y = glm::length(glm::vec3(m_worldMatrix[1][0], m_worldMatrix[1][1], m_worldMatrix[1][2]));
	m_worldScale.z = glm::length(glm::vec3(m_worldMatrix[2][0], m_worldMatrix[2][1], m_worldMatrix[2][2]));

	glm::vec3 rotationX { 0.0f };
	glm::vec3 rotationY { 0.0f };
	glm::vec3 rotationZ { 0.0f };

	if (m_worldScale.x)
	{
		rotationX = (glm::vec3(m_worldMatrix[0][0], m_worldMatrix[0][1], m_worldMatrix[0][2]) / m_worldScale.x);
	}
	if (m_worldScale.y)
	{
		rotationY = (glm::vec3(m_worldMatrix[1][0], m_worldMatrix[1][1], m_worldMatrix[1][2]) / m_worldScale.y);
	}
	if (m_worldScale.z)
	{
		rotationZ = (glm::vec3(m_worldMatrix[2][0], m_worldMatrix[2][1], m_worldMatrix[2][2]) / m_worldScale.z);
	}

	glm::mat3 worldRotationMatrix { rotationX.x, rotationX.y, rotationX.z,
									rotationY.x, rotationY.y, rotationY.z,
									rotationZ.x, rotationZ.y, rotationZ.z };

	glm::fquat rotation = glm::quat_cast(worldRotationMatrix);
	m_worldRotation = glm::eulerAngles(rotation) * (180.0f / glm::pi<float>());
}