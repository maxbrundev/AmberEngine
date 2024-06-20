#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/ContextualMenuWindow.h"

void AmberEngine::UI::Widgets::ContextualMenuWindow::Close()
{
	if (enabled)
		ImGui::CloseCurrentPopup();
}

void AmberEngine::UI::Widgets::ContextualMenuWindow::DrawImplementation()
{
	if (ImGui::BeginPopupContextWindow(m_widgetID.c_str(), ImGuiPopupFlags_NoOpenOverItems | ImGuiPopupFlags_MouseButtonRight))
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}
