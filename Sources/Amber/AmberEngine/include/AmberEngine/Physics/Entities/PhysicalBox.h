#pragma once

#include "AmberEngine/Physics/Entities/PhysicalObject.h"

#include "AmberEngine/Maths/Transform.h"

namespace AmberEngine::Physics::Entities
{
	class PhysicalBox : public PhysicalObject
	{
	public:
		PhysicalBox(const glm::vec3& p_size = {0.5f, 0.5f, 0.5f});
		PhysicalBox(Maths::Transform& p_transform, const glm::vec3& p_size = { 0.5f, 0.5f, 0.5f });

		~PhysicalBox() override = default;

		void SetSize(const glm::vec3& p_size);
		glm::vec3 GetSize() const;

	private:
		void CreateCollisionShape(const glm::vec3& p_size);
		void RecreateCollisionShape(const glm::vec3& p_size);
		virtual void SetLocalScaling(const glm::vec3& p_scaling) override;

	private:
		glm::vec3 m_size;
	};
}