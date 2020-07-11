#pragma once

#include <string>

namespace AmberEngine::Settings
{
	struct WindowSettings
	{
		const char* title;
		
		uint16_t width;
		uint16_t height;
		
		bool fullScreen = false;
		bool resizable	= true;
		bool vsync		= true;

		uint8_t samples = 4;
	};
}
