#pragma once

#include <vector>


namespace AmberTools::Analytics::Profiling
{
	struct ProfilerReport final
	{
		struct Action final
		{
			std::string name;
			double duration;
			double percentage;
			uint64_t calls;
		};

		double				elaspedTime		= 0.0;
		uint16_t			workingThreads	= 0u;
		uint32_t			elapsedFrames	= 0u;
		std::vector<Action> actions;
	};
}