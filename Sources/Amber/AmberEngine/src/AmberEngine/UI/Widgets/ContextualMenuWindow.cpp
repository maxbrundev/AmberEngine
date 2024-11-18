#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/ContextualMenuWindow.h"

void AmberEngine::UI::Widgets::ContextualMenuWindow::Draw()
{
	if (ImGui::BeginPopupContextWindow(m_widgetID.c_str(), ImGuiMouseButton_Right, false))
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}

void AmberEngine::UI::Widgets::ContextualMenuWindow::DrawImplementation()
{

}
