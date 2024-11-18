/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <string>
#include <chrono>

/**
* This macro allow the creation of profiler spies
* Any spy will die and send data to the profiler at
* the end of the scope where this macro get called
*/
#define PROFILER_SPY(name)\
		std::unique_ptr<AmberEngine::Analytics::Profiling::ProfilerSpy> __profiler_spy__ = \
		AmberEngine::Analytics::Profiling::Profiler::IsEnabled() ? std::make_unique<AmberEngine::Analytics::Profiling::ProfilerSpy>(name) : nullptr

namespace AmberEngine::Analytics::Profiling
{
	/**
	* A little informer that is created when PROFILER_SPY(name) is written.
	* It helps collecting informations about methods durations for debugging
	* event in release
	*/
	struct ProfilerSpy final
	{
		/**
		* Create the profiler spy with the given name.
		* @param p_name
		*/
		ProfilerSpy(const std::string& p_name);

		/**
		* Destroy the profiler spy.
		* On destruction, his collected data will be saved in the profiler
		*/
		~ProfilerSpy();

		const	std::string								name;
		const	std::chrono::steady_clock::time_point	start;
				std::chrono::steady_clock::time_point	end;
	};
}