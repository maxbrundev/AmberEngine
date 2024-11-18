#include "Amberpch.h"

#include "AmberEngine/Physics/Entities/PhysicalCapsule.h"
#include "AmberEngine/Physics/Tools/Conversion.h"

AmberEngine::Physics::Entities::PhysicalCapsule::PhysicalCapsule(float p_radius, float p_height) : PhysicalObject()
{
	CreateCollisionShape(p_radius, p_height);
	Init();
}

AmberEngine::Physics::Entities::PhysicalCapsule::PhysicalCapsule(Maths::Transform & p_transform, float p_radius, float p_height) : PhysicalObject(p_transform)
{
	CreateCollisionShape(p_radius, p_height);
	Init();
}

void AmberEngine::Physics::Entities::PhysicalCapsule::SetRadius(float p_radius)
{
	RecreateCollisionShape(p_radius, m_height);
}

void AmberEngine::Physics::Entities::PhysicalCapsule::SetHeight(float p_height)
{
	RecreateCollisionShape(m_radius, p_height);
}

float AmberEngine::Physics::Entities::PhysicalCapsule::GetRadius() const
{
	return m_radius;
}

float AmberEngine::Physics::Entities::PhysicalCapsule::GetHeight() const
{
	return m_height;
}

void AmberEngine::Physics::Entities::PhysicalCapsule::CreateCollisionShape(float p_radius, float p_height)
{
	m_shape = std::make_unique<btCapsuleShape>(p_radius, p_height);
	m_radius = p_radius;
	m_height = p_height;
}

void AmberEngine::Physics::Entities::PhysicalCapsule::RecreateCollisionShape(float p_radius, float p_height)
{
	CreateCollisionShape(p_radius, p_height);
	RecreateBody();
}

void AmberEngine::Physics::Entities::PhysicalCapsule::SetLocalScaling(const glm::vec3 & p_scaling)
{
	m_shape->setLocalScaling({ std::max(p_scaling.x, p_scaling.z), p_scaling.y, 1.0f });
}