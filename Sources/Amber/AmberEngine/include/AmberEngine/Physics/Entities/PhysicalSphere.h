#pragma once

#include "AmberEngine/Physics/Entities/PhysicalObject.h"

namespace AmberEngine::Physics::Entities
{
	class PhysicalSphere : public PhysicalObject
	{
	public:
		PhysicalSphere(float p_radius = 1.0f);
		PhysicalSphere(Maths::Transform& p_transform, float p_radius = 1.0f);
		~PhysicalSphere() override = default;

		void SetRadius(float p_radius);

		float GetRadius() const;

	private:
		void CreateCollisionShape(float p_radius);
		void RecreateCollisionShape(float p_radius);
		virtual void SetLocalScaling(const glm::vec3& p_scaling) override;

	private:
		float m_radius;
	};
}