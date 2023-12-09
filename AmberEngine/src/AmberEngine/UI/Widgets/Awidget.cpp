#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/AWidget.h"

uint64_t AmberEngine::UI::Widgets::AWidget::__WIDGET_ID_INCREMENT = 0;

AmberEngine::UI::Widgets::AWidget::AWidget()
{
	m_widgetID = "##" + std::to_string(__WIDGET_ID_INCREMENT++);
}

void AmberEngine::UI::Widgets::AWidget::Draw()
{
	if (enabled)
	{
		DrawImplementation();

		if (!lineBreak)
			ImGui::SameLine();
	}
}

void AmberEngine::UI::Widgets::AWidget::LinkTo(const AWidget& p_widget)
{
	m_widgetID = p_widget.m_widgetID;
}

void AmberEngine::UI::Widgets::AWidget::Destroy()
{
	m_destroyed = true;
}

bool AmberEngine::UI::Widgets::AWidget::IsDestroyed() const
{
	return m_destroyed;
}

void AmberEngine::UI::Widgets::AWidget::SetParent(WidgetContainer* p_parent)
{
	m_parent = p_parent;
}

bool AmberEngine::UI::Widgets::AWidget::HasParent() const
{
	return m_parent != nullptr;
}

AmberEngine::UI::WidgetContainer* AmberEngine::UI::Widgets::AWidget::GetParent() const
{
	return m_parent;
}
