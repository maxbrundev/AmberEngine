#include "Amberpch.h"

#include "AmberMaths/Transform.h"

AmberMaths::Transform::Transform(const glm::vec3& p_localPosition, const glm::vec3& p_localRotation, const glm::vec3& p_localScale) :
m_notificationHandlerID(-1),
m_parent(nullptr)
{
	GenerateMatricesLocal(p_localPosition, p_localRotation, p_localScale);
}

AmberMaths::Transform::Transform(const Transform& p_other) :
Transform(p_other.m_worldPosition, p_other.m_worldRotationEuler, p_other.m_worldScale)
{
}

AmberMaths::Transform& AmberMaths::Transform::operator=(const Transform& p_other)
{
	GenerateMatricesWorld(p_other.m_worldPosition, p_other.m_worldRotationEuler, p_other.m_worldScale);

	return *this;
}

AmberMaths::Transform::~Transform()
{
	Notifier.NotifyChildren(TransformNotifier::ENotification::TRANSFORM_DESTROYED);
}

void AmberMaths::Transform::GenerateMatricesWorld(const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale)
{
	glm::mat4 positionMatrix(1.0f);
	glm::mat4 rotationMatrix(1.0f);
	glm::mat4 scaleMatrix(1.0f);

	glm::mat4 rotationX(1.0f);
	rotationX = glm::rotate(rotationX, glm::radians(p_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 rotationY(1.0f);
	rotationY = glm::rotate(rotationY, glm::radians(p_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 rotationZ(1.0f);
	rotationZ = glm::rotate(rotationZ, glm::radians(p_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	rotationMatrix = rotationZ * rotationY * rotationX;

	m_worldMatrix = glm::translate(positionMatrix, p_position) * rotationMatrix * glm::scale(scaleMatrix, p_scale);
	m_worldPosition	= p_position;
	m_worldRotationEuler = p_rotation;
	m_worldScale = p_scale;
	m_worldRotation = glm::quat_cast(rotationMatrix);

	UpdateLocalMatrix();
}

void AmberMaths::Transform::GenerateMatricesLocal(const glm::vec3& p_position, const glm::vec3& p_rotation, const glm::vec3& p_scale)
{
	glm::mat4 positionMatrix(1.0f);
	glm::mat4 rotationMatrix(1.0f);
	glm::mat4 scaleMatrix(1.0f);

	glm::mat4 rotationX(1.0f);
	rotationX = glm::rotate(rotationX, glm::radians(p_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 rotationY(1.0f);
	rotationY = glm::rotate(rotationY, glm::radians(p_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 rotationZ(1.0f);
	rotationZ = glm::rotate(rotationZ, glm::radians(p_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	rotationMatrix = rotationZ * rotationY * rotationX;

	m_localMatrix = glm::translate(positionMatrix, p_position) * rotationMatrix * glm::scale(scaleMatrix, p_scale);
	m_localPosition = p_position;
	m_localRotationEuler = p_rotation;
	m_localScale = p_scale;
	m_localRotation = glm::quat_cast(rotationMatrix);

	UpdateWorldMatrix();
}

void AmberMaths::Transform::UpdateWorldMatrix()
{
	m_worldMatrix = HasParent() ? m_parent->m_worldMatrix * m_localMatrix : m_localMatrix;
	PreDecomposeWorldMatrix();

	Notifier.NotifyChildren(TransformNotifier::ENotification::TRANSFORM_CHANGED);
}

void AmberMaths::Transform::UpdateLocalMatrix()
{
	m_localMatrix = HasParent() ? glm::inverse(m_parent->m_worldMatrix) * m_worldMatrix : m_worldMatrix;
	PreDecomposeLocalMatrix();

	Notifier.NotifyChildren(TransformNotifier::ENotification::TRANSFORM_CHANGED);
}

bool AmberMaths::Transform::HasParent() const
{
	return m_parent != nullptr;
}

void AmberMaths::Transform::TranslateLocal(const glm::vec3& p_translation)
{
	SetLocalPosition(m_localPosition + p_translation);
}

void AmberMaths::Transform::RotateLocal(const glm::vec3& p_rotation)
{
	SetLocalRotationEuler(m_localRotationEuler + p_rotation);
}

void AmberMaths::Transform::ScaleLocal(const glm::vec3& p_scale)
{
	SetLocalScale(m_localScale * p_scale);
}

void AmberMaths::Transform::SetParent(Transform& p_parent)
{
	m_parent = &p_parent;

	m_notificationHandlerID = m_parent->Notifier.AddNotificationHandler(std::bind(&Transform::NotificationHandler, this, std::placeholders::_1));

	UpdateWorldMatrix();
}

bool AmberMaths::Transform::RemoveParent()
{
	if(m_parent != nullptr)
	{
		m_parent->Notifier.RemoveNotificationHandler(m_notificationHandlerID);
		m_parent = nullptr;

		UpdateWorldMatrix();

		return true;
	}

	return false;
}

void AmberMaths::Transform::NotificationHandler(TransformNotifier::ENotification p_notification)
{
	switch (p_notification)
	{
	case TransformNotifier::ENotification::TRANSFORM_CHANGED:
		UpdateWorldMatrix();
		break;

	case TransformNotifier::ENotification::TRANSFORM_DESTROYED:
		GenerateMatricesLocal(m_worldPosition, m_worldRotationEuler, m_worldScale);
		m_parent = nullptr;
		UpdateWorldMatrix();
		break;
	}
}

void AmberMaths::Transform::SetLocalPosition(const glm::vec3& p_newPosition)
{
	GenerateMatricesLocal(p_newPosition, m_localRotationEuler, m_localScale);
}

void AmberMaths::Transform::SetLocalRotationEuler(const glm::vec3& p_newRotation)
{
	GenerateMatricesLocal(m_localPosition, p_newRotation, m_localScale);
}

void AmberMaths::Transform::SetLocalRotation(const glm::quat& p_newRotation)
{
	GenerateMatricesLocal(m_localPosition, glm::degrees(glm::eulerAngles(p_newRotation)), m_localScale);
}

void AmberMaths::Transform::SetLocalScale(const glm::vec3& p_newScale)
{
	GenerateMatricesLocal(m_localPosition, m_localRotationEuler, p_newScale);
}

void AmberMaths::Transform::SetWorldPosition(const glm::vec3& p_newPosition)
{
	GenerateMatricesWorld(p_newPosition, m_worldRotationEuler, m_worldScale);
}

void AmberMaths::Transform::SetWorldRotationEuler(const glm::vec3& p_newRotation)
{
	GenerateMatricesWorld(m_worldPosition, p_newRotation, m_worldScale);
}

void AmberMaths::Transform::SetWorldRotation(const glm::quat& p_newRotation)
{
	GenerateMatricesWorld(m_worldPosition, glm::degrees(glm::eulerAngles(p_newRotation)), m_worldScale);
}

void AmberMaths::Transform::SetWorldScale(const glm::vec3& p_newScale)
{
	GenerateMatricesWorld(m_worldPosition, m_worldRotationEuler, p_newScale);
}

const glm::vec3& AmberMaths::Transform::GetLocalPosition() const
{
	return m_localPosition;
}

const glm::vec3& AmberMaths::Transform::GetLocalRotationEuler() const
{
	return m_localRotationEuler;
}

const glm::quat& AmberMaths::Transform::GetLocalRotation() const
{
	return m_localRotation;
}

const glm::vec3& AmberMaths::Transform::GetLocalScale() const
{
	return m_localScale;
}

const glm::vec3& AmberMaths::Transform::GetWorldPosition() const
{
	return m_worldPosition;
}

const glm::vec3& AmberMaths::Transform::GetWorldRotationEuler() const
{
	return m_worldRotationEuler;
}

const glm::quat& AmberMaths::Transform::GetWorldRotation() const
{
	return m_worldRotation;
}

const glm::vec3& AmberMaths::Transform::GetWorldScale() const
{
	return m_worldScale;
}

glm::vec3 AmberMaths::Transform::GetWorldForward() const
{
	const glm::vec3 forward = m_worldRotation * glm::vec3(0.0f, 0.0f, 1.0f);

	return forward;
}

glm::vec3 AmberMaths::Transform::GetWorldUp() const
{
	return m_worldRotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 AmberMaths::Transform::GetWorldRight() const
{
	return m_worldRotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 AmberMaths::Transform::GetLocalForward() const
{
	const glm::vec3 forward = m_localRotation * glm::vec3(0.0f, 0.0f, 1.0f);

	return forward;
}

glm::vec3 AmberMaths::Transform::GetLocalUp() const
{
	return m_localRotation * glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 AmberMaths::Transform::GetLocalRight() const
{
	return m_localRotation * glm::vec3(1.0f, 0.0f, 0.0f);
}

const glm::mat4& AmberMaths::Transform::GetLocalMatrix() const
{
	return m_localMatrix;
}

const glm::mat4& AmberMaths::Transform::GetWorldMatrix() const
{
	return m_worldMatrix;
}

void AmberMaths::Transform::PreDecomposeWorldMatrix()
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

	glm::vec3 rotationX(0.0f);
	glm::vec3 rotationY(0.0f);
	glm::vec3 rotationZ(0.0f);

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

	const glm::mat3 worldRotationMatrix(rotationX.x, rotationX.y, rotationX.z, 
										rotationY.x, rotationY.y, rotationY.z, 
										rotationZ.x, rotationZ.y, rotationZ.z);
	
	m_worldRotation = glm::quat_cast(worldRotationMatrix);
	
	m_worldRotationEuler = glm::eulerAngles(m_worldRotation) * (180.0f / glm::pi<float>());
}

void AmberMaths::Transform::PreDecomposeLocalMatrix()
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

	glm::vec3 rotationX(0.0f);
	glm::vec3 rotationY(0.0f);
	glm::vec3 rotationZ(0.0f);

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

	const glm::mat3 localRotationMatrix(rotationX.x, rotationX.y, rotationX.z, 
										rotationY.x, rotationY.y, rotationY.z, 
										rotationZ.x, rotationZ.y, rotationZ.z);

	m_localRotation = glm::quat_cast(localRotationMatrix);

	m_localRotationEuler = glm::eulerAngles(m_localRotation) * (180.0f / glm::pi<float>());
}
