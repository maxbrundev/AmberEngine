#include "Amberpch.h"

#include <algorithm>

#include "AmberEngine/Physics/Entities/PhysicalSphere.h"

AmberEngine::Physics::Entities::PhysicalSphere::PhysicalSphere(float p_radius) : PhysicalObject()
{
	CreateCollisionShape(p_radius);
	Init();
}

AmberEngine::Physics::Entities::PhysicalSphere::PhysicalSphere(Maths::Transform & p_transform, float p_radius) : PhysicalObject(p_transform)
{
	CreateCollisionShape(p_radius);
	Init();
}

void AmberEngine::Physics::Entities::PhysicalSphere::SetRadius(float p_radius)
{
	if (p_radius != m_radius)
		RecreateCollisionShape(p_radius);
}

float AmberEngine::Physics::Entities::PhysicalSphere::GetRadius() const
{
	return m_radius;
}

void AmberEngine::Physics::Entities::PhysicalSphere::CreateCollisionShape(float p_radius)
{
	m_shape = std::make_unique<btSphereShape>(p_radius);
	m_radius = p_radius;
}

void AmberEngine::Physics::Entities::PhysicalSphere::RecreateCollisionShape(float p_radius)
{
	CreateCollisionShape(p_radius);
	RecreateBody();
}

void AmberEngine::Physics::Entities::PhysicalSphere::SetLocalScaling(const glm::vec3 & p_scaling)
{
	float radiusScale = std::max(std::max(p_scaling.x, p_scaling.y), p_scaling.z);
	m_shape->setLocalScaling({ radiusScale, radiusScale, radiusScale });
}
