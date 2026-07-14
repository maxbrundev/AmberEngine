#pragma once


#include "AmberUI/Panels/APanelWindow.h"

namespace AmberEditor::Panels
{
	class HelpWindow : public AmberUI::Panels::APanelWindow
	{
	public:
		HelpWindow(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings);
		virtual ~HelpWindow() override = default;
	};
}
