
#include "Amberpch.h"
#include "AmberTools/Analytics/Profiling/ProfilerSpy.h"
#include "AmberTools/Analytics/Profiling/Profiler.h"

AmberTools::Analytics::Profiling::ProfilerSpy::ProfilerSpy(const std::string & p_name) :
	name(p_name),
	start(std::chrono::steady_clock::now())
{

}

AmberTools::Analytics::Profiling::ProfilerSpy::~ProfilerSpy()
{
	end = std::chrono::steady_clock::now();
	AmberTools::Analytics::Profiling::Profiler::Save(*this);
}