#pragma once

#include "AmberPhysics/Entities/PhysicalObject.h"

namespace AmberPhysics::Entities
{
	struct RaycastHit
	{
		PhysicalObject* FirstResultObject = nullptr;
		std::vector<PhysicalObject*> ResultObjects;
	};
}