#pragma once

#include "AmberEngine/API/Export.h"

namespace  AmberEngine::Settings
{
	struct API_AMBERENGINE DriverSettings
	{
		bool enableDepthTest = true;
		bool enableBackFaceCulling = true;
		bool enableBlend = true;
		bool enableMultisample = true;
		bool enableDebugCallback = true;
	};
}