#include "Amberpch.h"

#include "AmberUI/Panels/PanelUndecorated.h"

void AmberUI::Panels::PanelUndecorated::DrawImplementation()
{
	auto& style = ImGui::GetStyle();
	ImVec2 previousPadding = style.WindowPadding;
	ImVec2 previousMinSize = style.WindowMinSize;
	style.WindowPadding = { 0, 0 };
	style.WindowMinSize = { 0, 0 };

	if (ImGui::Begin(m_panelID.c_str(), nullptr, CollectFlags()))
	{
		style.WindowPadding = previousPadding;
		style.WindowMinSize = previousMinSize;

		Update();

		DrawWidgets();

		ImGui::End();
	}
	else
	{
		style.WindowPadding = previousPadding;
		style.WindowMinSize = previousMinSize;
	}
}

int AmberUI::Panels::PanelUndecorated::CollectFlags()
{
	ImGuiWindowFlags flags =
		ImGuiWindowFlags_AlwaysUseWindowPadding |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoScrollWithMouse |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoBackground;

	if (AutoSize) flags |= ImGuiWindowFlags_AlwaysAutoResize;

	return flags;
}
