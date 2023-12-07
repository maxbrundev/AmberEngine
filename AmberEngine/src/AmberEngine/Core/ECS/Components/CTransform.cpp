#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Components/CTransform.h"

#include "AmberEngine/UI/GUIDrawer.h"

AmberEngine::Core::ECS::Components::CTransform::CTransform(Actor& p_owner, glm::vec3 p_localPosition,
                                                           glm::vec3 p_localRotation, glm::vec3 p_localScale) : AComponent(p_owner)
{
	m_transform.GenerateMatrices(p_localPosition, p_localRotation, p_localScale);
}

void AmberEngine::Core::ECS::Components::CTransform::SetParent(CTransform& p_parent)
{
	m_transform.SetParent(p_parent.GetTransform());
}

bool AmberEngine::Core::ECS::Components::CTransform::RemoveParent()
{
	return m_transform.RemoveParent();
}

bool AmberEngine::Core::ECS::Components::CTransform::HasParent() const
{
	return m_transform.HasParent();
}

void AmberEngine::Core::ECS::Components::CTransform::SetLocalPosition(glm::vec3 p_newPosition)
{
	m_transform.SetLocalPosition(p_newPosition);
}

void AmberEngine::Core::ECS::Components::CTransform::SetLocalRotation(glm::vec3 p_newRotation)
{
	m_transform.SetLocalRotation(p_newRotation);
}

void AmberEngine::Core::ECS::Components::CTransform::SetLocalScale(glm::vec3 p_newScale)
{
	m_transform.SetLocalScale(p_newScale);
}

void AmberEngine::Core::ECS::Components::CTransform::SetWorldPosition(glm::vec3 p_newPosition)
{
	m_transform.SetWorldPosition(p_newPosition);
}

void AmberEngine::Core::ECS::Components::CTransform::SetWorldRotation(glm::vec3 p_newRotation)
{
	m_transform.SetWorldRotation(p_newRotation);
}

void AmberEngine::Core::ECS::Components::CTransform::SetWorldScale(glm::vec3 p_newScale)
{
	m_transform.SetWorldScale(p_newScale);
}

void AmberEngine::Core::ECS::Components::CTransform::TranslateLocal(const glm::vec3& p_translation)
{
	m_transform.TranslateLocal(p_translation);
}

void AmberEngine::Core::ECS::Components::CTransform::RotateLocal(const glm::vec3& p_rotation)
{
	m_transform.RotateLocal(p_rotation);
}

void AmberEngine::Core::ECS::Components::CTransform::ScaleLocal(const glm::vec3& p_scale)
{
	m_transform.ScaleLocal(p_scale);
}

const glm::vec3& AmberEngine::Core::ECS::Components::CTransform::GetLocalPosition() const
{
	return m_transform.GetLocalPosition();
}

const glm::vec3& AmberEngine::Core::ECS::Components::CTransform::GetLocalRotation() const
{
	return m_transform.GetLocalRotationEuler();
}

const glm::vec3& AmberEngine::Core::ECS::Components::CTransform::GetLocalScale() const
{
	return m_transform.GetLocalScale();
}

const glm::vec3& AmberEngine::Core::ECS::Components::CTransform::GetWorldPosition() const
{
	return m_transform.GetWorldPosition();
}

const glm::vec3& AmberEngine::Core::ECS::Components::CTransform::GetWorldRotation() const
{
	return m_transform.GetWorldRotationEuler();
}

const glm::vec3& AmberEngine::Core::ECS::Components::CTransform::GetWorldScale() const
{
	return m_transform.GetWorldScale();
}

const glm::mat4& AmberEngine::Core::ECS::Components::CTransform::GetLocalMatrix() const
{
	return m_transform.GetLocalMatrix();
}

const glm::mat4& AmberEngine::Core::ECS::Components::CTransform::GetWorldMatrix() const
{
	return m_transform.GetWorldMatrix();
}

glm::vec3 AmberEngine::Core::ECS::Components::CTransform::GetWorldForward() const
{
	return m_transform.GetWorldForward();
}

glm::vec3 AmberEngine::Core::ECS::Components::CTransform::GetWorldUp() const
{
	return m_transform.GetWorldUp();
}

glm::vec3 AmberEngine::Core::ECS::Components::CTransform::GetWorldRight() const
{
	return m_transform.GetWorldRight();
}

glm::vec3 AmberEngine::Core::ECS::Components::CTransform::GetLocalForward() const
{
	return m_transform.GetLocalForward();
}

glm::vec3 AmberEngine::Core::ECS::Components::CTransform::GetLocalUp() const
{
	return m_transform.GetLocalUp();
}

glm::vec3 AmberEngine::Core::ECS::Components::CTransform::GetLocalRight() const
{
	return m_transform.GetLocalRight();
}

AmberEngine::Maths::Transform& AmberEngine::Core::ECS::Components::CTransform::GetTransform()
{
	return m_transform;
}

std::string AmberEngine::Core::ECS::Components::CTransform::GetName()
{
	return "Transform";
}

void AmberEngine::Core::ECS::Components::CTransform::OnInspector(AmberEngine::UI::WidgetContainer& p_root)
{
	UI::GUIDrawer::DrawVec3(p_root, "Position", std::bind(&CTransform::GetLocalPosition, this), std::bind(&CTransform::SetLocalPosition, this, std::placeholders::_1), 0.05f);
	UI::GUIDrawer::DrawVec3(p_root, "Rotation", std::bind(&CTransform::GetLocalRotation, this), std::bind(&CTransform::SetLocalRotation, this, std::placeholders::_1), 0.05f);
	UI::GUIDrawer::DrawVec3(p_root, "Scale", std::bind(&CTransform::GetLocalScale, this), std::bind(&CTransform::SetLocalScale, this, std::placeholders::_1), 0.05f, 0.0001f);
}
