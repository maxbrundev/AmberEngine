#pragma once

#include <chrono>

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Utils
{
	class API_AMBERENGINE Time
	{
	private:
		static void Initialize();

		static std::chrono::steady_clock::time_point	__START_TIME;
		static std::chrono::steady_clock::time_point	__LAST_TIME;
		static std::chrono::steady_clock::time_point	__CURRENT_TIME;

		static std::chrono::duration<double>			__ELAPSED;
		static float									__TIME_SINCE_START;
		static bool		__INITIALIZED;
		static float	__TIME_SCALE;
		static float	__DELTA_TIME;

	public:
		void Update();

		static float GetFramerate();
		static float GetDeltaTime();
		static float GetDeltaTimeUnscaled();
		static float GetTimeSinceStart();
		static float GetTimeScale();
		static void Scale(float p_coeff);
		static void SetTimeScale(float p_timeScale);
	};
}