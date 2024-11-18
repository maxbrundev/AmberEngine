#pragma once

#include "AmberEngine/Analytics/Hardware/CPUInfo.h"
#include "AmberEngine/Analytics/Hardware/GPUInfo.h"
#include "AmberEngine/Analytics/Hardware/HardwareReport.h"
#include "AmberEngine/Analytics/Hardware/RAMInfo.h"

namespace AmberEngine::Analytics::Hardware
{
	/**
	* The hardware info class will gather informations about the hardware in real-time (CPU usage, GPU usage, RAM usage...)
	*/
	class HardwareInfo final
	{
	public:
		/**
		* Constructor of the HardwareInfo class
		* @param p_timeInterval
		*/
		HardwareInfo(double p_timeInterval = 1.0);

		/**
		* Generate a report containing information about the actual hardware
		*/
		HardwareReport GenerateReport();

		/**
		* Update hardware information (CPU usage, GPU usage, RAM)
		*/
		void Tick();

	private:
		void Update();

		double m_timeInterval;
		double m_timer;

		std::chrono::steady_clock::time_point	m_previousTime;
		std::chrono::steady_clock::time_point	m_currentTime;
		std::chrono::duration<double>			m_elapsedTime;

		float m_cpuUsage;
		float m_gpuUsage;

		CPUInfo m_cpuInfo;
		GPUInfo m_gpuInfo;
		RAMInfo m_ramInfo;
	};
}
