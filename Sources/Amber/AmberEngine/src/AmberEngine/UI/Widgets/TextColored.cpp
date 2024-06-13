#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/TextColored.h"

AmberEngine::UI::Widgets::TextColored::TextColored(const std::string& p_content, const AmberEngine::Data::Color& p_color): Text(p_content), color(p_color)
{
}

void AmberEngine::UI::Widgets::TextColored::DrawImplementation()
{
	ImGui::TextColored(ImVec4(color.r, color.g, color.b, color.a), content.c_str());
}
