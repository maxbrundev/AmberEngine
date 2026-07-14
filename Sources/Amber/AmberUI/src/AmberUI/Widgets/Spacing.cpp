#include "Amberpch.h"

#include "AmberUI/Widgets/Spacing.h"

AmberUI::Widgets::Spacing::Spacing(uint16_t p_spaces) : Spaces(p_spaces)
{

}

void AmberUI::Widgets::Spacing::DrawImplementation()
{
	for (uint16_t i = 0; i < Spaces; ++i)
	{
		ImGui::Spacing();

		if (i + 1 < Spaces)
			ImGui::SameLine();
	}
}
