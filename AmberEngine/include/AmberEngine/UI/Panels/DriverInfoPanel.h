#pragma once

#include "AmberEngine/UI/Panels/APanelWindow.h"

namespace AmberEngine::UI::Panels
{
	class DriverInfoPanel : public APanelWindow
	{
	public:
		DriverInfoPanel(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings);
		virtual ~DriverInfoPanel() override = default;

	private:
		std::string m_info;
	};
}

