#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/ContextualMenu.h"

void AmberEngine::UI::Widgets::ContextualMenu::DrawImplementation()
{
	if (ImGui::BeginPopupContextItem())
	{
		DrawWidgets();
		ImGui::EndPopup();
	}
}
