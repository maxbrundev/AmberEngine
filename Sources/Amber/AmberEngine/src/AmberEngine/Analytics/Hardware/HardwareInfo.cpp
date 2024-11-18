/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "Amberpch.h"

#include "AmberEngine/Analytics/Hardware/HardwareInfo.h"

AmberEngine::Analytics::Hardware::HardwareInfo::HardwareInfo(double p_timeInterval) :
	m_timeInterval(p_timeInterval),
	m_timer(0.0f),
	m_cpuUsage(0.0f),
	m_gpuUsage(0.0f)
{
}

AmberEngine::Analytics::Hardware::HardwareReport AmberEngine::Analytics::Hardware::HardwareInfo::GenerateReport()
{
	return HardwareReport
	{
		m_cpuUsage,
		m_gpuUsage,
		m_ramInfo.GetUsedRAM(),
		m_ramInfo.GetFreeRAM(),
		m_ramInfo.GetMaxRAM()
	};
}

void AmberEngine::Analytics::Hardware::HardwareInfo::Tick()
{
	m_currentTime = std::chrono::steady_clock::now();
	m_elapsedTime = m_currentTime - m_previousTime;

	m_timer += m_elapsedTime.count();

	if (m_timer >= m_timeInterval)
	{
		while (m_timer >= m_timeInterval)
			m_timer -= m_timeInterval;

		Update();
	}

	m_previousTime = m_currentTime;
}

void AmberEngine::Analytics::Hardware::HardwareInfo::Update()
{
	m_cpuUsage = m_cpuInfo.CalculateCPULoad();
	m_gpuUsage = m_gpuInfo.CalculateGPULoad();

	m_ramInfo.Update();
}
