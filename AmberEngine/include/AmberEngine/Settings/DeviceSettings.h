#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Settings
{
	struct API_AMBERENGINE DeviceSettings
	{
		const char* title;

		int contextVersionMajor = 4;
		int contextVersionMinor = 4;

		bool enableResizable = true;
		bool enableDebugContext = true;
		bool enableVsync = true;
		bool enableDebugCallback = true;
		bool enableFrameBufferSizeCallback = true;
	};
}