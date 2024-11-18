/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/


#include "Amberpch.h"
#include "AmberEngine/Analytics/Profiling/ProfilerSpy.h"
#include "AmberEngine/Analytics/Profiling/Profiler.h"

AmberEngine::Analytics::Profiling::ProfilerSpy::ProfilerSpy(const std::string & p_name) :
	name(p_name),
	start(std::chrono::steady_clock::now())
{

}

AmberEngine::Analytics::Profiling::ProfilerSpy::~ProfilerSpy()
{
	end = std::chrono::steady_clock::now();
	AmberEngine::Analytics::Profiling::Profiler::Save(*this);
}