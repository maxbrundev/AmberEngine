#pragma once

#include "AmberUI/Panels/APanelWindow.h"

namespace AmberEditor::Panels
{
	class DriverInfoPanel : public AmberUI::Panels::APanelWindow
	{
	public:
		DriverInfoPanel(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings);
		virtual ~DriverInfoPanel() override = default;

	private:
		std::string m_info;
	};
}

