#include "Amberpch.h"

#include "AmberEngine/Maths/Transform.h"

#include "AmberEngine/Eventing/Event.h"

AmberEngine::Maths::Transform::Transform(const glm::vec3 & p_position, const glm::vec3 & p_rotation, const glm::vec3 & p_scale) : m_parent(nullptr)
{
	GenerateMatricesLocal(p_position, p_rotation, p_scale);
}

AmberEngine::Maths::Transform::~Transform()
{
	for (auto& [childID, callback] : m_childrenTransformCallbacks)
	{
		callback(ETransformState::DESTROYED);
		callback = nullptr;
	}

	m_childrenTransformCallbacks.clear();
}

void AmberEngine::Maths::Transform::GenerateMatricesWorld(glm::vec3 p_position, glm::vec3 p_rotation, glm::vec3 p_scale)
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

	m_worldMatrix =  glm::translate(positionMatrix, p_position) * rotationMatrix * glm::scale(scaleMatrix, p_scale);
	m_worldPosition	= p_position;
	m_worldRotationEuler = p_rotation;
	m_worldScale	= p_scale;
	m_worldRotation = glm::quat_cast(rotationMatrix);

	UpdateLocalMatrix();
}

void AmberEngine::Maths::Transform::GenerateMatricesLocal(glm::vec3 p_position, glm::vec3 p_rotation, glm::vec3 p_scale)
{
	glm::mat4 positionMatrix{ 1.0f };
	glm::mat4 rotationMatrix{ 1.0f };
	glm::mat4 scaleMatrix{ 1.0f };

	glm::mat4 rotationX{ 1.0f };
	rotationX = glm::rotate(rotationX, glm::radians(p_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 rotationY{ 1.0f };
	rotationY = glm::rotate(rotationY, glm::radians(p_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 rotationZ{ 1.0f };
	rotationZ = glm::rotate(rotationZ, glm::radians(p_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	rotationMatrix = rotationZ * rotationY * rotationX;

	m_localMatrix = glm::translate(positionMatrix, p_position) * rotationMatrix * glm::scale(scaleMatrix, p_scale);
	m_localPosition = p_position;
	m_localRotationEuler = p_rotation;
	m_localScale = p_scale;
	m_localRotation = glm::quat_cast(rotationMatrix);

	UpdateWorldMatrix();
}

void AmberEngine::Maths::Transform::UpdateWorldMatrix()
{
	m_worldMatrix = HasParent() ? m_parent->m_worldMatrix * m_localMatrix : m_localMatrix;
	PreDecomposeWorldMatrix();

	for (const auto& [childID, callback] : m_childrenTransformCallbacks)
	{
		callback(ETransformState::CHANGED);
	}
}

void AmberEngine::Maths::Transform::UpdateLocalMatrix()
{
	m_localMatrix = HasParent() ? glm::inverse(m_parent->m_worldMatrix) * m_worldMatrix : m_worldMatrix;
	PreDecomposeLocalMatrix();

	for (const auto&[childID, callback] : m_childrenTransformCallbacks)
	{
		callback(ETransformState::CHANGED);
	}
}

bool AmberEngine::Maths::Transform::HasParent() const
{
	return m_parent != nullptr;
}

void AmberEngine::Maths::Transform::TranslateLocal(const glm::vec3& p_translation)
{
	SetLocalPosition(m_localPosition + p_translation);
}

void AmberEngine::Maths::Transform::RotateLocal(const glm::vec3& p_rotation)
{
	SetLocalRotationEuler(m_localRotationEuler + p_rotation);
}

void AmberEngine::Maths::Transform::ScaleLocal(const glm::vec3& p_scale)
{
	SetLocalScale(m_localScale * p_scale);
}

void AmberEngine::Maths::Transform::SetParent(Transform& p_parent)
{
	m_parent = &p_parent;

	m_parentCallbackID = m_parent->AddChildrenCallback([this](auto&& p_state)
	{
		HandleParentTransformCallback(std::forward<decltype(p_state)>(p_state));
	});

	UpdateWorldMatrix();
}

bool AmberEngine::Maths::Transform::RemoveParent()
{
	if(m_parent != nullptr)
	{
		m_parent->m_childrenTransformCallbacks.erase(m_parentCallbackID);
		m_parent = nullptr;

		UpdateWorldMatrix();

		return true;
	}

	return false;
}

void AmberEngine::Maths::Transform::HandleParentTransformCallback(ETransformState p_state)
{
	switch (p_state)
	{
	case ETransformState::CHANGED:
		UpdateWorldMatrix();
		break;

	case ETransformState::DESTROYED:
		m_parent = nullptr;
		UpdateWorldMatrix();
		break;
	}
}

uint64_t AmberEngine::Maths::Transform::AddChildrenCallback(const std::function<void(ETransformState)>& p_callback)
{
	uint64_t childCallbackID = m_childCallbackID++;
	m_childrenTransformCallbacks.emplace(childCallbackID, p_callback);

	return childCallbackID;
}

void AmberEngine::Maths::Transform::SetLocalPosition(glm::vec3 p_newPosition)
{
	GenerateMatricesLocal(p_newPosition, m_localRotationEuler, m_localScale);
}

void AmberEngine::Maths::Transform::SetLocalRotationEuler(glm::vec3 p_newRotation)
{
	GenerateMatricesLocal(m_localPosition, p_newRotation, m_localScale);
}

void AmberEngine::Maths::Transform::SetLocalRotation(glm::quat p_newRotation)
{
	GenerateMatricesLocal(m_localPosition, glm::degrees(glm::eulerAngles(p_newRotation)), m_localScale);
}

void AmberEngine::Maths::Transform::SetLocalScale(glm::vec3 p_newScale)
{
	GenerateMatricesLocal(m_localPosition, m_localRotationEuler, p_newScale);
}

void AmberEngine::Maths::Transform::SetWorldPosition(glm::vec3 p_newPosition)
{
	GenerateMatricesWorld(p_newPosition, m_worldRotationEuler, m_worldScale);
}

void AmberEngine::Maths::Transform::SetWorldRotationEuler(glm::vec3 p_newRotation)
{
	GenerateMatricesWorld(m_worldPosition, p_newRotation, m_worldScale);
}

void AmberEngine::Maths::Transform::SetWorldRotation(glm::quat p_newRotation)
{
	GenerateMatricesWorld(m_worldPosition, glm::degrees(glm::eulerAngles(p_newRotation)), m_worldScale);
}

void AmberEngine::Maths::Transform::SetWorldScale(glm::vec3 p_newScale)
{
	GenerateMatricesWorld(m_worldPosition, m_worldRotationEuler, p_newScale);
}

const glm::vec3& AmberEngine::Maths::Transform::GetLocalPosition() const
{
	return m_localPosition;
}

const glm::vec3& AmberEngine::Maths::Transform::GetLocalRotationEuler() const
{
	return m_localRotationEuler;
}

const glm::quat& AmberEngine::Maths::Transform::GetLocalRotation() const
{
	return m_localRotation;
}

const glm::vec3& AmberEngine::Maths::Transform::GetLocalScale() const
{
	return m_localScale;
}

const glm::vec3& AmberEngine::Maths::Transform::GetWorldPosition() const
{
	return m_worldPosition;
}

const glm::vec3& AmberEngine::Maths::Transform::GetWorldRotationEuler() const
{
	return m_worldRotationEuler;
}

const glm::quat& AmberEngine::Maths::Transform::GetWorldRotation() const
{
	return m_worldRotation;
}

const glm::vec3& AmberEngine::Maths::Transform::GetWorldScale() const
{
	return m_worldScale;
}

glm::vec3 AmberEngine::Maths::Transform::GetWorldForward() const
{
	const glm::vec3 forward = m_worldRotation * glm::vec3(0.0f, 0.0f, 1.0f);

	return forward;
}

glm::vec3 AmberEngine::Maths::Transform::GetWorldUp() const
{
	return m_worldRotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 AmberEngine::Maths::Transform::GetWorldRight() const
{
	return m_worldRotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 AmberEngine::Maths::Transform::GetLocalForward() const
{
	const glm::vec3 forward = m_localRotation * glm::vec3(0.0f, 0.0f, 1.0f);

	return forward;
}

glm::vec3 AmberEngine::Maths::Transform::GetLocalUp() const
{
	return m_localRotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 AmberEngine::Maths::Transform::GetLocalRight() const
{
	return m_localRotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

const glm::mat4& AmberEngine::Maths::Transform::GetLocalMatrix() const
{
	return m_localMatrix;
}

const glm::mat4& AmberEngine::Maths::Transform::GetWorldMatrix() const
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
	m_worldScale.y = glm::length(scaleY);
	m_worldScale.z = glm::length(scaleZ);

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

	const glm::mat3 worldRotationMatrix { rotationX.x, rotationX.y, rotationX.z,
										  rotationY.x, rotationY.y, rotationY.z,
										  rotationZ.x, rotationZ.y, rotationZ.z };
	
	m_worldRotation = glm::quat_cast(worldRotationMatrix);
	
	m_worldRotationEuler = glm::eulerAngles(m_worldRotation) * (180.0f / glm::pi<float>());
}

void AmberEngine::Maths::Transform::PreDecomposeLocalMatrix()
{
	m_localPosition.x = m_localMatrix[3][0];
	m_localPosition.y = m_localMatrix[3][1];
	m_localPosition.z = m_localMatrix[3][2];

	glm::vec3 scaleX(m_localMatrix[0][0], m_localMatrix[0][1], m_localMatrix[0][2]);
	glm::vec3 scaleY(m_localMatrix[1][0], m_localMatrix[1][1], m_localMatrix[1][2]);
	glm::vec3 scaleZ(m_localMatrix[2][0], m_localMatrix[2][1], m_localMatrix[2][2]);

	m_localScale.x = glm::length(scaleX);
	m_localScale.y = glm::length(scaleY);
	m_localScale.z = glm::length(scaleZ);

	glm::vec3 rotationX{ 0.0f };
	glm::vec3 rotationY{ 0.0f };
	glm::vec3 rotationZ{ 0.0f };

	if (m_localScale.x)
	{
		rotationX = (glm::vec3(m_localMatrix[0][0], m_localMatrix[0][1], m_localMatrix[0][2]) / m_localScale.x);
	}
	if (m_localScale.y)
	{
		rotationY = (glm::vec3(m_localMatrix[1][0], m_localMatrix[1][1], m_localMatrix[1][2]) / m_localScale.y);
	}
	if (m_localScale.z)
	{
		rotationZ = (glm::vec3(m_localMatrix[2][0], m_localMatrix[2][1], m_localMatrix[2][2]) / m_localScale.z);
	}

	const glm::mat3 localRotationMatrix{ rotationX.x, rotationX.y, rotationX.z,
										  rotationY.x, rotationY.y, rotationY.z,
										  rotationZ.x, rotationZ.y, rotationZ.z };

	m_localRotation = glm::quat_cast(localRotationMatrix);

	m_localRotationEuler = glm::eulerAngles(m_localRotation) * (180.0f / glm::pi<float>());
}
