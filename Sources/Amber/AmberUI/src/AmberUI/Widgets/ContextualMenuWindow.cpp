#include "Amberpch.h"

#include "AmberUI/Widgets/ContextualMenuWindow.h"

void AmberUI::Widgets::ContextualMenuWindow::Draw()
{
	if (ImGui::BeginPopupContextWindow(m_widgetID.c_str(), ImGuiMouseButton_Right, false))
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}

void AmberUI::Widgets::ContextualMenuWindow::DrawImplementation()
{

}
