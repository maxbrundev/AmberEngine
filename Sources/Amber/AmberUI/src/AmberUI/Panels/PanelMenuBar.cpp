#include "Amberpch.h"

#include "AmberUI/Panels/PanelMenuBar.h"

void AmberUI::Panels::PanelMenuBar::DrawImplementation()
{
	if (!m_widgets.empty() && ImGui::BeginMainMenuBar())
	{
		DrawWidgets();
		ImGui::EndMainMenuBar();
	}
}
