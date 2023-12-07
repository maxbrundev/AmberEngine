#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/Button.h"

#include "AmberEngine/ImGUI/imgui_internal.h"

AmberEngine::UI::Widgets::Buttons::Button::Button(const std::string& p_label, const glm::vec2& p_size, bool p_disabled) : label(p_label), size(p_size), disabled(p_disabled)
{
	const auto& style = ImGui::GetStyle();

	idleBackgroundColor    = Data::Color(style.Colors[ImGuiCol_Button].x, style.Colors[ImGuiCol_Button].y, style.Colors[ImGuiCol_Button].z);
	hoveredBackgroundColor =  Data::Color(style.Colors[ImGuiCol_ButtonHovered].x, style.Colors[ImGuiCol_ButtonHovered].y, style.Colors[ImGuiCol_ButtonHovered].z);
	clickedBackgroundColor = Data::Color(style.Colors[ImGuiCol_ButtonActive].x, style.Colors[ImGuiCol_ButtonActive].y, style.Colors[ImGuiCol_ButtonActive].z);
	textColor              = Data::Color(style.Colors[ImGuiCol_Text].x, style.Colors[ImGuiCol_Text].y, style.Colors[ImGuiCol_Text].z);
}

void AmberEngine::UI::Widgets::Buttons::Button::DrawImplementation()
{
	auto& style = ImGui::GetStyle();

	const auto defaultIdleColor    = style.Colors[ImGuiCol_Button];
	const auto defaultHoveredColor = style.Colors[ImGuiCol_ButtonHovered];
	const auto defaultClickedColor = style.Colors[ImGuiCol_ButtonActive];
	const auto defaultTextColor    = style.Colors[ImGuiCol_Text];

	style.Colors[ImGuiCol_Button]        = ImVec4(idleBackgroundColor.r, idleBackgroundColor.g, idleBackgroundColor.b, idleBackgroundColor.a);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(hoveredBackgroundColor.r, hoveredBackgroundColor.g, hoveredBackgroundColor.b, hoveredBackgroundColor.a);
	style.Colors[ImGuiCol_ButtonActive]  = ImVec4(clickedBackgroundColor.r, clickedBackgroundColor.g, clickedBackgroundColor.b, clickedBackgroundColor.a);
	style.Colors[ImGuiCol_Text]          = ImVec4(textColor.r, textColor.g, textColor.b, textColor.a);

	if (ImGui::ButtonEx((label + m_widgetID).c_str(), ImVec2(size.x, size.y), disabled ? ImGuiButtonFlags_Disabled : 0))
		ClickedEvent.Invoke();

	style.Colors[ImGuiCol_Button] = defaultIdleColor;
	style.Colors[ImGuiCol_ButtonHovered] = defaultHoveredColor;
	style.Colors[ImGuiCol_ButtonActive] = defaultClickedColor;
	style.Colors[ImGuiCol_Text] = defaultTextColor;
}
