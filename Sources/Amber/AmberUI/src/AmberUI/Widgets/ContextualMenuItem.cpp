#include "Amberpch.h"

#include "AmberUI/Widgets/ContextualMenuItem.h"

void AmberUI::Widgets::ContextualMenuItem::Draw()
{
	if (ImGui::BeginPopupContextItem())
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}
