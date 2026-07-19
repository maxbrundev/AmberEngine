#pragma once

#include <unordered_map>
#include <chrono>
#include <mutex>


#include "AmberTools/Analytics/Profiling/ProfilerReport.h"

namespace AmberTools::Analytics::Profiling
{
	/* Forward declaration of the profiler spy structure */
	struct ProfilerSpy;

	class Profiler final
	{
	public:
		Profiler();

		ProfilerReport GenerateReport();

		void ClearHistory();

		void Update(float p_deltaTime);

		static void Save(ProfilerSpy& p_spy);

		static bool IsEnabled();

		static void ToggleEnable();

		static void Enable();

		static void Disable();

	private:
		/* Time relatives */
		std::chrono::steady_clock::time_point m_lastTime;

		/* Profiler settings */
		static bool __ENABLED;

		/* Collected data */
		static std::mutex									__SAVE_MUTEX;
		static std::unordered_map<std::string, double>		__ELPASED_HISTORY;
		static std::unordered_map<std::string, uint64_t>	__CALLS_COUNTER;
		static std::vector<std::thread::id>					__WORKING_THREADS;
		static uint32_t										__ELAPSED_FRAMES;
	};
}