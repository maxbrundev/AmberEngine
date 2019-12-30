#pragma once

#include <cstdint>

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Settings
{
	struct API_AMBERENGINE WindowSettings
	{
		const char* title;
		
		uint16_t width;
		uint16_t height;
		
		bool enableResizable = true;
		bool enableVsync = true;
		bool enableDebugCallback = true;
		bool enableFrameBufferSizeCallback = true;

		uint8_t samples = 4;
	};
}
