#pragma once

#include "AmberEngine/Physics/Entities/PhysicalObject.h"

namespace AmberEngine::Physics::Entities
{
	struct RaycastHit
	{
		PhysicalObject* FirstResultObject = nullptr;
		std::vector<PhysicalObject*> ResultObjects;
	};
}