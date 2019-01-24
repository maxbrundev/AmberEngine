#pragma once

#include <queue>

#include "AmberEngine/API/Export.h"

namespace  AmberEngine::Core
{
	struct API_AMBERENGINE DriverSettings
	{
		bool enableDepthTest;
		bool enableBlend;
		bool enableBackFaceCulling;
		bool enableMultisample;
		bool enableDebugCallback;
	};

	class Driver
	{
	private:
		std::queue<std::string> m_errors;

	public:
		Driver(const DriverSettings& p_settings);
		~Driver() = default;

		void DisplayErrors();
		void DisplayDriverInfo();

		const std::queue<std::string>& GetQueueErros();
	};
}
