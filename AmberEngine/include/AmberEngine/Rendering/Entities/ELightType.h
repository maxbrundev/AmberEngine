#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Rendering::Entities
{
	enum class API_AMBERENGINE ELightType
	{
		POINT,
		DIRECTIONAL,
		SPOT,
		AMBIENT
	};
}