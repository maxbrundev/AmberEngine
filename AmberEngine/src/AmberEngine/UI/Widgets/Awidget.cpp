#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/AWidget.h"

uint64_t AmberEngine::UI::AWidget::__WIDGET_ID_INCREMENT = 0;

AmberEngine::UI::AWidget::AWidget()
{
	m_widgetID = "##" + std::to_string(__WIDGET_ID_INCREMENT++);
}

void AmberEngine::UI::AWidget::Draw()
{
	if (enabled)
	{
		DrawImplementation();

		if (!lineBreak)
			ImGui::SameLine();
	}
}

void AmberEngine::UI::AWidget::LinkTo(const AWidget& p_widget)
{
	m_widgetID = p_widget.m_widgetID;
}

void AmberEngine::UI::AWidget::Destroy()
{
	m_destroyed = true;
}

bool AmberEngine::UI::AWidget::IsDestroyed() const
{
	return m_destroyed;
}

void AmberEngine::UI::AWidget::SetParent(WidgetContainer* p_parent)
{
	m_parent = p_parent;
}

bool AmberEngine::UI::AWidget::HasParent() const
{
	return m_parent != nullptr;
}

AmberEngine::UI::WidgetContainer* AmberEngine::UI::AWidget::GetParent() const
{
	return m_parent;
}
