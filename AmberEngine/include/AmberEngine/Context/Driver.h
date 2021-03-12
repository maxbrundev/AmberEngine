#pragma once

#include <GL/glew.h>

#include "AmberEngine/Settings/DriverSettings.h"

namespace  AmberEngine::Context
{
	class Driver
	{
	public:
		Driver(const Settings::DriverSettings& p_driverSettings);
		~Driver() = default;

		void DisplayDriverInfo();
		
	private:
		void InitGlew();

	private:
		bool isActive;
	};
}
