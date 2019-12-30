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
		
		bool resizable = true;
		bool fullScreen	= true;
		bool vsync = true;
		bool enableFrameBufferSizeCallback = true;

		uint8_t samples = 4;
	};
}
