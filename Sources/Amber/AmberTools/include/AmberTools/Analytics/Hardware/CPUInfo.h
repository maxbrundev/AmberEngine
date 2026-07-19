#pragma once

namespace AmberTools::Analytics::Hardware
{
	class CPUInfo final
	{
	public:
		float CalculateCPULoad();

	private:
		float CalculateCPULoad(uint64_t idleTicks, uint64_t totalTicks);

	private:
		uint64_t m_cpuPreviousTotalTicks = 0;
		uint64_t m_cpuPreviousIdleTicks = 0;
	};
}