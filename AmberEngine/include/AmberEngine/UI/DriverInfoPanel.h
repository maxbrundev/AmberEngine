#pragma once

#include "AmberEngine/UI/Panels/APanelWindow.h"

class DriverInfoPanel : public AmberEngine::UI::APanelWindow
{
public:
	DriverInfoPanel(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings);
	~DriverInfoPanel() override = default;

protected:
	void DrawContent() override;

private:
	std::string m_info;
};
