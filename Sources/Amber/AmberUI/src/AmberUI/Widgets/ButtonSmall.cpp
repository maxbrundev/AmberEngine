#include "Amberpch.h"

#include "AmberUI/Widgets/ButtonSmall.h"

#include "AmberUI/Tools/Converter.h"

AmberUI::Widgets::ButtonSmall::ButtonSmall(const std::string& p_label) : Label(p_label)
{
	const auto& style = ImGui::GetStyle();

	IdleBackgroundColor = AmberUI::Tools::Converter::ToColor(style.Colors[ImGuiCol_Button]);
	HoveredBackgroundColor = AmberUI::Tools::Converter::ToColor(style.Colors[ImGuiCol_ButtonHovered]);
	ClickedBackgroundColor = AmberUI::Tools::Converter::ToColor(style.Colors[ImGuiCol_ButtonActive]);
	TextColor = AmberUI::Tools::Converter::ToColor(style.Colors[ImGuiCol_Text]);
}

void AmberUI::Widgets::ButtonSmall::DrawImplementation()
{
	auto& style = ImGui::GetStyle();

	const auto defaultIdleColor = style.Colors[ImGuiCol_Button];
	const auto defaultHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
	const auto defaultClickedColor = style.Colors[ImGuiCol_ButtonActive];
	const auto defaultTextColor = style.Colors[ImGuiCol_Text];

	style.Colors[ImGuiCol_Button] = AmberUI::Tools::Converter::ToImVec4(IdleBackgroundColor);
	style.Colors[ImGuiCol_ButtonHovered] = AmberUI::Tools::Converter::ToImVec4(HoveredBackgroundColor);
	style.Colors[ImGuiCol_ButtonActive] = AmberUI::Tools::Converter::ToImVec4(ClickedBackgroundColor);
	style.Colors[ImGuiCol_Text] = AmberUI::Tools::Converter::ToImVec4(TextColor);

	if (ImGui::SmallButton((Label + m_widgetID).c_str()))
		ClickedEvent.Invoke();

	style.Colors[ImGuiCol_Button] = defaultIdleColor;
	style.Colors[ImGuiCol_ButtonHovered] = defaultHoveredColor;
	style.Colors[ImGuiCol_ButtonActive] = defaultClickedColor;
	style.Colors[ImGuiCol_Text] = defaultTextColor;
}
