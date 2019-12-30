#pragma once

#include <cstdint>
#include "AmberEngine/API/Export.h"

namespace AmberEngine::Settings
{
	struct API_AMBERENGINE DeviceSettings
	{
		uint8_t contextVersionMajor = 4;
		uint8_t contextVersionMinor = 3;

		bool debugProfile = true;
	};
}