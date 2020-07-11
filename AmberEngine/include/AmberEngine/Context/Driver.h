#pragma once

#include <queue>

#include "AmberEngine/Settings/DriverSettings.h"

namespace  AmberEngine::Context
{
	class Driver
	{
	private:
		bool isActive;
		std::queue<std::string> m_errors;

	public:
		Driver(const Settings::DriverSettings& p_driverSettings);
		~Driver() = default;

		void DisplayDriverInfo();
		
		void DisplayErrors();
		const std::queue<std::string>& GetQueueErros();

	private:
		void InitGlew();
	};
}
