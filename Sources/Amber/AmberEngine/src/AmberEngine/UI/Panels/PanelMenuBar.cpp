#include "Amberpch.h"

#include "AmberEngine/UI/Panels/PanelMenuBar.h"

void AmberEngine::UI::Panels::PanelMenuBar::DrawImplementation()
{
	if (!m_widgets.empty() && ImGui::BeginMainMenuBar())
	{
		DrawWidgets();
		ImGui::EndMainMenuBar();
	}
}
