#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/ButtonSmall.h"

#include "AmberEngine/Tools/Utils/Converter.h"

AmberEngine::UI::Widgets::ButtonSmall::ButtonSmall(const std::string& p_label) : label(p_label)
{
	const auto& style = ImGui::GetStyle();

	idleBackgroundColor = Utils::Converter::ToColor(style.Colors[ImGuiCol_Button]);
	hoveredBackgroundColor = Utils::Converter::ToColor(style.Colors[ImGuiCol_ButtonHovered]);
	clickedBackgroundColor = Utils::Converter::ToColor(style.Colors[ImGuiCol_ButtonActive]);
	textColor = Utils::Converter::ToColor(style.Colors[ImGuiCol_Text]);
}

void AmberEngine::UI::Widgets::ButtonSmall::DrawImplementation()
{
	auto& style = ImGui::GetStyle();

	const auto defaultIdleColor = style.Colors[ImGuiCol_Button];
	const auto defaultHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
	const auto defaultClickedColor = style.Colors[ImGuiCol_ButtonActive];
	const auto defaultTextColor = style.Colors[ImGuiCol_Text];

	style.Colors[ImGuiCol_Button] = Utils::Converter::ToImVec4(idleBackgroundColor);
	style.Colors[ImGuiCol_ButtonHovered] = Utils::Converter::ToImVec4(hoveredBackgroundColor);
	style.Colors[ImGuiCol_ButtonActive] = Utils::Converter::ToImVec4(clickedBackgroundColor);
	style.Colors[ImGuiCol_Text] = Utils::Converter::ToImVec4(textColor);

	if (ImGui::SmallButton((label + m_widgetID).c_str()))
		ClickedEvent.Invoke();

	style.Colors[ImGuiCol_Button] = defaultIdleColor;
	style.Colors[ImGuiCol_ButtonHovered] = defaultHoveredColor;
	style.Colors[ImGuiCol_ButtonActive] = defaultClickedColor;
	style.Colors[ImGuiCol_Text] = defaultTextColor;
}
