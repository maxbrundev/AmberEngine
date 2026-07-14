#include "Amberpch.h"

#include "AmberUI/DragDrop.h"

AmberUI::DragDropSource::DragDropSource(const std::string& p_identifier, const std::string& p_tooltip, const std::pair<std::string, Widgets::Group*>& p_data) :
Identifier(p_identifier),
Tooltip(p_tooltip),
Data(p_data)
{
}

void AmberUI::DragDropSource::Draw()
{
	ImGuiDragDropFlags src_flags = 0;
	src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
	src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

	if (!HasTooltip)
		src_flags |= ImGuiDragDropFlags_SourceNoPreviewTooltip;

	if (ImGui::BeginDragDropSource(src_flags))
	{
		if (!m_isDragged)
			DragStartEvent.Invoke();

		m_isDragged = true;

		if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
			ImGui::Text(Tooltip.c_str());
		ImGui::SetDragDropPayload(Identifier.c_str(), &Data, sizeof(Data));
		ImGui::EndDragDropSource();
	}
	else
	{
		if (m_isDragged)
			DragStopEvent.Invoke();

		m_isDragged = false;
	}
}

bool AmberUI::DragDropSource::IsDragged() const
{
	return m_isDragged;
}

AmberUI::DragDropTarget::DragDropTarget(const std::string& p_identifier) : Identifier(p_identifier)
{
}

void AmberUI::DragDropTarget::Draw()
{
	if (ImGui::BeginDragDropTarget())
	{
		if (!m_isHovered)
			HoverStartEvent.Invoke();

		m_isHovered = true;

		ImGuiDragDropFlags target_flags = 0;

		if (!ShowYellowRect)
			target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Identifier.c_str(), target_flags))
		{
			std::pair<std::string, Widgets::Group*> receivedData = *static_cast<std::pair<std::string, Widgets::Group*>*>(payload->Data);
			DataReceivedEvent.Invoke(receivedData);
		}
		ImGui::EndDragDropTarget();
	}
	else
	{
		if (m_isHovered)
			HoverEndEvent.Invoke();

		m_isHovered = false;
	}
}

bool AmberUI::DragDropTarget::IsHovered() const
{
	return m_isHovered;
}
