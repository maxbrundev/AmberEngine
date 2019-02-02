#pragma once

#include <queue>

#include "AmberEngine/Settings/DriverSettings.h"

namespace  AmberEngine::Context
{
	class Driver
	{
	private:
		std::queue<std::string> m_errors;

	public:
		Driver(const Settings::DriverSettings& p_settings);
		~Driver() = default;

		void DisplayErrors();
		void DisplayDriverInfo();

		const std::queue<std::string>& GetQueueErros();
	};
}
