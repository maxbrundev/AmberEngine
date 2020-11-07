#pragma once

namespace AmberEngine::Settings
{
	struct DeviceSettings
	{
		uint8_t contextVersionMajor = 4;
		uint8_t contextVersionMinor = 3;

		bool debugProfile = true;
	};
}