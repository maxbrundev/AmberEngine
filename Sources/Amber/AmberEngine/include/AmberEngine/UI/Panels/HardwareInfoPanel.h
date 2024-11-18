#pragma once

#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/Analytics/Hardware/HardwareInfo.h"
#include "AmberEngine/UI/Widgets/APlot.h"

namespace AmberEngine::UI::Panels
{
	class HardwareInfoPanel : public APanelWindow
	{
	public:
		HardwareInfoPanel
		(
			const std::string& p_title,
			bool p_opened,
			const PanelSettings& p_windowSettings,
			float p_logFrequency,
			size_t p_maxElements
		);

		/**
		* Destructor
		*/
		~HardwareInfoPanel();

		/**
		* Update hardware info
		* @param p_deltaTime
		*/
		void Update(float p_deltaTime);

	private:
		float p_updateTimer = 0.f;
		float m_logFrequency;
		size_t m_maxElements;
		Widgets::APlot* m_cpuUsage;
		Widgets::APlot* m_gpuUsage;
		Widgets::APlot* m_ramUsage;
		AmberEngine::Analytics::Hardware::HardwareInfo* m_hardwareInfo;
	};
}