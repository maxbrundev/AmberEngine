#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/Text.h"

AmberEngine::UI::Widgets::Text::Text(const std::string& p_content) : DataWidget(content), content(p_content)
{
}

void AmberEngine::UI::Widgets::Text::DrawImplementation()
{
	ImGui::Text(content.c_str());
}
