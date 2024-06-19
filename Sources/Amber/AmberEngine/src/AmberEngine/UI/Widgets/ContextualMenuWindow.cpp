#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/ContextualMenuWindow.h"

void AmberEngine::UI::Widgets::ContextualMenuWindow::Close()
{
	if (enabled)
		ImGui::CloseCurrentPopup();
}

void AmberEngine::UI::Widgets::ContextualMenuWindow::DrawImplementation()
{
	if (ImGui::BeginPopupContextWindow())
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}
