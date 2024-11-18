#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/Spacing.h"

AmberEngine::UI::Widgets::Spacing::Spacing(uint16_t p_spaces) : spaces(p_spaces)
{

}

void AmberEngine::UI::Widgets::Spacing::DrawImplementation()
{
	for (uint16_t i = 0; i < spaces; ++i)
	{
		ImGui::Spacing();

		if (i + 1 < spaces)
			ImGui::SameLine();
	}
}
