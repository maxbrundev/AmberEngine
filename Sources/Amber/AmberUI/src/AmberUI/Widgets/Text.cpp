#include "Amberpch.h"

#include "AmberUI/Widgets/Text.h"

AmberUI::Widgets::Text::Text(const std::string& p_content) : DataWidget(Content), Content(p_content)
{
}

void AmberUI::Widgets::Text::DrawImplementation()
{
	ImGui::Text(Content.c_str());
}
