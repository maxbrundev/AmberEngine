#pragma once

#include "AmberEngine/Settings/DeviceSettings.h"
#include "AmberEngine/Settings/DriverSettings.h"

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Settings
{
	struct API_AMBERENGINE RenderingSettings
	{
		Settings::DeviceSettings deviceSettings;
		Settings::DriverSettings driverSettings;
	};
}
