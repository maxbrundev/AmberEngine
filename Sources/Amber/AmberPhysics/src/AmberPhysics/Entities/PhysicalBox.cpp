#include "Amberpch.h"

#include "AmberPhysics/Entities/PhysicalBox.h"
#include "AmberPhysics/Tools/Conversion.h"

AmberPhysics::Entities::PhysicalBox::PhysicalBox(const glm::vec3& p_size) : PhysicalObject()
{
	CreateCollisionShape(p_size);
	Init();
}

AmberPhysics::Entities::PhysicalBox::PhysicalBox(AmberMaths::Transform & p_transform, const glm::vec3& p_size) : PhysicalObject(p_transform)
{
	CreateCollisionShape(p_size);
	Init();
}

void AmberPhysics::Entities::PhysicalBox::SetSize(const glm::vec3& p_size)
{
	if (m_size != p_size)
		RecreateCollisionShape(p_size);
}

glm::vec3 AmberPhysics::Entities::PhysicalBox::GetSize() const
{
	return m_size;
}

void AmberPhysics::Entities::PhysicalBox::CreateCollisionShape(const glm::vec3& p_size)
{
	m_shape = std::make_unique<btBoxShape>(AmberPhysics::Tools::Conversion::ToBtVector3(p_size));
	m_size = p_size;
}

void AmberPhysics::Entities::PhysicalBox::RecreateCollisionShape(const glm::vec3& p_size)
{
	CreateCollisionShape(p_size);
	RecreateBody();
}

void AmberPhysics::Entities::PhysicalBox::SetLocalScaling(const glm::vec3 & p_scaling)
{
	m_shape->setLocalScaling(AmberPhysics::Tools::Conversion::ToBtVector3(p_scaling));
}