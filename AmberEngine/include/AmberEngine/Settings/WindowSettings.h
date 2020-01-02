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
		
		bool fullScreen	= false;
		bool resizable = true;
		bool vsync = true;

		uint8_t samples = 4;
	};
}
