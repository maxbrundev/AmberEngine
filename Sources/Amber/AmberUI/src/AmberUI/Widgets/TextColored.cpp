#include "Amberpch.h"

#include "AmberUI/Widgets/TextColored.h"

AmberUI::Widgets::TextColored::TextColored(const std::string& p_content, const AmberRendering::Data::Color& p_color): Text(p_content), Color(p_color)
{
}

void AmberUI::Widgets::TextColored::DrawImplementation()
{
	ImGui::TextColored(ImVec4(Color.r, Color.g, Color.b, Color.a), Content.c_str());
}
