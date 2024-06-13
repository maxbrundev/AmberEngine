#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	enum class API_AMBERENGINE ETextureFilteringMode : uint16_t
	{
		NEAREST                = 0x2600,
		LINEAR                 = 0x2601,
		NEAREST_MIPMAP_NEAREST = 0x2700,
		LINEAR_MIPMAP_LINEAR   = 0x2703,
		LINEAR_MIPMAP_NEAREST  = 0x2701,
		NEAREST_MIPMAP_LINEAR  = 0x2702
	};
}