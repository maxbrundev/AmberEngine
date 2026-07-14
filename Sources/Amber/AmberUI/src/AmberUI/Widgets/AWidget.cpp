#include "Amberpch.h"

#include "AmberUI/Widgets/AWidget.h"

uint64_t AmberUI::Widgets::AWidget::WIDGET_ID_INCREMENT = 0;

AmberUI::Widgets::AWidget::AWidget()
{
	m_widgetID = "##" + std::to_string(WIDGET_ID_INCREMENT++);
}

void AmberUI::Widgets::AWidget::Draw()
{
	if (Enabled)
	{
		DrawImplementation();

		if (m_autoExecuteDragDrop)
		{
			DrawnEvent.Invoke();
			DrawDragDrop();
		}

		if (!LineBreak)
			ImGui::SameLine();
	}
}

void AmberUI::Widgets::AWidget::LinkTo(const AWidget& p_widget)
{
	m_widgetID = p_widget.m_widgetID;
}

void AmberUI::Widgets::AWidget::Destroy()
{
	m_destroyed = true;
}

bool AmberUI::Widgets::AWidget::IsDestroyed() const
{
	return m_destroyed;
}

void AmberUI::Widgets::AWidget::SetParent(WidgetContainer* p_parent)
{
	m_parent = p_parent;
}

bool AmberUI::Widgets::AWidget::HasParent() const
{
	return m_parent != nullptr;
}

AmberUI::WidgetContainer* AmberUI::Widgets::AWidget::GetParent() const
{
	return m_parent;
}

AmberUI::DragDropSource& AmberUI::Widgets::AWidget::SetDragDropSource(const std::string& p_identifier, const std::string& p_tooltip, const std::pair<std::string, Group*>& p_data)
{
	m_dragDropSource = std::make_unique<DragDropSource>(p_identifier, p_tooltip, p_data);
	return *m_dragDropSource;
}

AmberUI::DragDropTarget& AmberUI::Widgets::AWidget::AddDragDropTarget(const std::string& p_identifier)
{
	m_dragDropTargets.push_back(std::make_unique<DragDropTarget>(p_identifier));
	return *m_dragDropTargets.back();
}

void AmberUI::Widgets::AWidget::ClearDragDrop()
{
	m_dragDropSource.reset();
	m_dragDropTargets.clear();
}

void AmberUI::Widgets::AWidget::DrawDragDrop()
{
	if (m_dragDropSource)
		m_dragDropSource->Draw();

	for (const std::unique_ptr<DragDropTarget>& target : m_dragDropTargets)
	{
		target->Draw();
	}
}
