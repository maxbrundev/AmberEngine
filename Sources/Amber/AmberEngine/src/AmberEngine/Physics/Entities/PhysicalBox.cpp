#include "Amberpch.h"

#include "AmberEngine/Physics/Entities/PhysicalBox.h"
#include "AmberEngine/Physics/Tools/Conversion.h"

AmberEngine::Physics::Entities::PhysicalBox::PhysicalBox(const glm::vec3& p_size) : PhysicalObject()
{
	CreateCollisionShape(p_size);
	Init();
}

AmberEngine::Physics::Entities::PhysicalBox::PhysicalBox(Maths::Transform & p_transform, const glm::vec3& p_size) : PhysicalObject(p_transform)
{
	CreateCollisionShape(p_size);
	Init();
}

void AmberEngine::Physics::Entities::PhysicalBox::SetSize(const glm::vec3& p_size)
{
	if (m_size != p_size)
		RecreateCollisionShape(p_size);
}

glm::vec3 AmberEngine::Physics::Entities::PhysicalBox::GetSize() const
{
	return m_size;
}

void AmberEngine::Physics::Entities::PhysicalBox::CreateCollisionShape(const glm::vec3& p_size)
{
	m_shape = std::make_unique<btBoxShape>(AmberEngine::Physics::Tools::Conversion::ToBtVector3(p_size));
	m_size = p_size;
}

void AmberEngine::Physics::Entities::PhysicalBox::RecreateCollisionShape(const glm::vec3& p_size)
{
	CreateCollisionShape(p_size);
	RecreateBody();
}

void AmberEngine::Physics::Entities::PhysicalBox::SetLocalScaling(const glm::vec3 & p_scaling)
{
	m_shape->setLocalScaling(AmberEngine::Physics::Tools::Conversion::ToBtVector3(p_scaling));
}