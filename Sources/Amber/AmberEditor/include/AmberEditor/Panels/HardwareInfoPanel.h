#pragma once

#include "AmberUI/Panels/APanelWindow.h"
#include "AmberTools/Analytics/Hardware/HardwareInfo.h"
#include "AmberUI/Widgets/APlot.h"

namespace AmberEditor::Panels
{
	class HardwareInfoPanel : public AmberUI::Panels::APanelWindow
	{
	public:
		HardwareInfoPanel
		(
			const std::string& p_title,
			bool p_opened,
			const AmberUI::Panels::PanelSettings& p_windowSettings,
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
		AmberUI::Widgets::APlot* m_cpuUsage;
		AmberUI::Widgets::APlot* m_gpuUsage;
		AmberUI::Widgets::APlot* m_ramUsage;
		AmberTools::Analytics::Hardware::HardwareInfo* m_hardwareInfo;
	};
}