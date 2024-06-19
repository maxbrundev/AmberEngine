#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/ContextualMenuItem.h"

void AmberEngine::UI::Widgets::ContextualMenuItem::Close()
{
	if(enabled)
		ImGui::CloseCurrentPopup();
}

void AmberEngine::UI::Widgets::ContextualMenuItem::DrawImplementation()
{
	if (ImGui::BeginPopupContextItem())
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}
