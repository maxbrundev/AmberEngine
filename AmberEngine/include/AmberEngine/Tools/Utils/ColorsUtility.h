#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Data/Color.h"

namespace AmberEngine::Utils
{
	struct API_AMBERENGINE ColorsUtility
	{
		ColorsUtility() = delete;

		static Data::Color RGBAToNormalized(Data::Color p_color);
		static Data::Color NormalizedToRGBA(Data::Color p_color);

		static uint32_t PackRGBAToNormalized(Data::Color p_color);
		static uint32_t PackNormalizedToRGBA(Data::Color p_color);
	};
}
