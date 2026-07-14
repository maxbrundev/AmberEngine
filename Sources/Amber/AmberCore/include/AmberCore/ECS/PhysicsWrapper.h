#pragma once

#include <vector>

#include "AmberPhysics/Entities/RaycastHit.h"

#include "AmberCore/ECS/Components/CPhysicalObject.h"

namespace AmberCore::ECS
{
	class PhysicsWrapper
	{
	public:
		struct RaycastHit
		{
			Components::CPhysicalObject* FirstResultObject = nullptr;
			std::vector<Components::CPhysicalObject*> ResultObjects;
		};

		static bool Raycast(glm::vec3 p_origin, glm::vec3 p_direction, float p_distance, RaycastHit& p_outHit);
	};
}
