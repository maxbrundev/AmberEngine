#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/ContextualMenuItem.h"

void AmberEngine::UI::Widgets::ContextualMenuItem::Draw()
{
	if (ImGui::BeginPopupContextItem())
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}
