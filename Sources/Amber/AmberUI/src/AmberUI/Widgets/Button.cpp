#include "Amberpch.h"

#include "AmberUI/Widgets/Button.h"

#include "imgui_internal.h"

AmberUI::Widgets::Button::Button(const std::string& p_label, const glm::vec2& p_size, bool p_disabled) : Label(p_label), Size(p_size), Disabled(p_disabled)
{
	const auto& style = ImGui::GetStyle();

	IdleBackgroundColor    = AmberRendering::Data::Color(style.Colors[ImGuiCol_Button].x, style.Colors[ImGuiCol_Button].y, style.Colors[ImGuiCol_Button].z);
	HoveredBackgroundColor =  AmberRendering::Data::Color(style.Colors[ImGuiCol_ButtonHovered].x, style.Colors[ImGuiCol_ButtonHovered].y, style.Colors[ImGuiCol_ButtonHovered].z);
	ClickedBackgroundColor = AmberRendering::Data::Color(style.Colors[ImGuiCol_ButtonActive].x, style.Colors[ImGuiCol_ButtonActive].y, style.Colors[ImGuiCol_ButtonActive].z);
	TextColor              = AmberRendering::Data::Color(style.Colors[ImGuiCol_Text].x, style.Colors[ImGuiCol_Text].y, style.Colors[ImGuiCol_Text].z);
}

void AmberUI::Widgets::Button::DrawImplementation()
{
	auto& style = ImGui::GetStyle();

	const auto defaultIdleColor    = style.Colors[ImGuiCol_Button];
	const auto defaultHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
	const auto defaultClickedColor = style.Colors[ImGuiCol_ButtonActive];
	const auto defaultTextColor    = style.Colors[ImGuiCol_Text];

	style.Colors[ImGuiCol_Button]        = ImVec4(IdleBackgroundColor.r, IdleBackgroundColor.g, IdleBackgroundColor.b, IdleBackgroundColor.a);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(HoveredBackgroundColor.r, HoveredBackgroundColor.g, HoveredBackgroundColor.b, HoveredBackgroundColor.a);
	style.Colors[ImGuiCol_ButtonActive]  = ImVec4(ClickedBackgroundColor.r, ClickedBackgroundColor.g, ClickedBackgroundColor.b, ClickedBackgroundColor.a);
	style.Colors[ImGuiCol_Text]          = ImVec4(TextColor.r, TextColor.g, TextColor.b, TextColor.a);

	if (ImGui::ButtonEx((Label + m_widgetID).c_str(), ImVec2(Size.x, Size.y), Disabled ? ImGuiButtonFlags_Disabled : 0))
		ClickedEvent.Invoke();

	style.Colors[ImGuiCol_Button] = defaultIdleColor;
	style.Colors[ImGuiCol_ButtonHovered] = defaultHoveredColor;
	style.Colors[ImGuiCol_ButtonActive] = defaultClickedColor;
	style.Colors[ImGuiCol_Text] = defaultTextColor;
}
