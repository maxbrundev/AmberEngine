#include "Amberpch.h"

#include <algorithm>

#include "AmberPhysics/Entities/PhysicalSphere.h"

AmberPhysics::Entities::PhysicalSphere::PhysicalSphere(float p_radius) : PhysicalObject()
{
	CreateCollisionShape(p_radius);
	Init();
}

AmberPhysics::Entities::PhysicalSphere::PhysicalSphere(AmberMaths::Transform & p_transform, float p_radius) : PhysicalObject(p_transform)
{
	CreateCollisionShape(p_radius);
	Init();
}

void AmberPhysics::Entities::PhysicalSphere::SetRadius(float p_radius)
{
	if (p_radius != m_radius)
		RecreateCollisionShape(p_radius);
}

float AmberPhysics::Entities::PhysicalSphere::GetRadius() const
{
	return m_radius;
}

void AmberPhysics::Entities::PhysicalSphere::CreateCollisionShape(float p_radius)
{
	m_shape = std::make_unique<btSphereShape>(p_radius);
	m_radius = p_radius;
}

void AmberPhysics::Entities::PhysicalSphere::RecreateCollisionShape(float p_radius)
{
	CreateCollisionShape(p_radius);
	RecreateBody();
}

void AmberPhysics::Entities::PhysicalSphere::SetLocalScaling(const glm::vec3 & p_scaling)
{
	float radiusScale = std::max(std::max(p_scaling.x, p_scaling.y), p_scaling.z);
	m_shape->setLocalScaling({ radiusScale, radiusScale, radiusScale });
}
