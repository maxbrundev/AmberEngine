#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	enum class API_AMBERENGINE ETextureType : uint8_t
	{
		DIFFUSE_MAP,
		SPECULAR_MAP,
		NORMAL_MAP,
		HEIGHT_MAP,
		MASK_MAP
	};
}