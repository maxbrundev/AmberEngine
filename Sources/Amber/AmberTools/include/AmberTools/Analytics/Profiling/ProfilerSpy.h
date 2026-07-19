#pragma once

#include <string>
#include <chrono>

#include "AmberTools/Analytics/Profiling/Profiler.h"

#define PROFILER_SPY(name)\
		std::unique_ptr<AmberTools::Analytics::Profiling::ProfilerSpy> __profiler_spy__ = \
		AmberTools::Analytics::Profiling::Profiler::IsEnabled() ? std::make_unique<AmberTools::Analytics::Profiling::ProfilerSpy>(name) : nullptr

namespace AmberTools::Analytics::Profiling
{
	struct ProfilerSpy final
	{
		ProfilerSpy(const std::string& p_name);

		~ProfilerSpy();

		const	std::string								name;
		const	std::chrono::steady_clock::time_point	start;
				std::chrono::steady_clock::time_point	end;
	};
}