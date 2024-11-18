#pragma once

#include "AmberEngine/Physics/Entities/PhysicalObject.h"

#include "AmberEngine/Maths/Transform.h"

namespace AmberEngine::Physics::Entities
{
	class PhysicalCapsule : public PhysicalObject
	{
	public:
		
		PhysicalCapsule(float p_radius = 1.0f, float p_height = 1.0f);
		PhysicalCapsule(Maths::Transform& p_transform, float p_radius = 1.0f, float p_height = 1.0f);

		~PhysicalCapsule() override = default;

		void SetRadius(float p_radius);

		void SetHeight(float p_height);

		float GetRadius() const;

		float GetHeight() const;

	private:
		void CreateCollisionShape(float p_radius, float p_height);
		void RecreateCollisionShape(float p_radius, float p_height);
		virtual void SetLocalScaling(const glm::vec3& p_scaling) override;

	private:
		float m_radius;
		float m_height;
	};
}