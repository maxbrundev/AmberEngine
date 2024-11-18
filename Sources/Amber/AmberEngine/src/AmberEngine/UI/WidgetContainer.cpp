#include "Amberpch.h"

#include "AmberEngine/UI/WidgetContainer.h"

AmberEngine::UI::WidgetContainer::~WidgetContainer()
{
	RemoveAllWidgets();
}

void AmberEngine::UI::WidgetContainer::RemoveWidget(Widgets::AWidget& p_widget)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::unique_ptr<Widgets::AWidget>& p_instance)
	{
		return p_instance.get() == &p_widget;
	});

	if (found != m_widgets.end())
	{
		p_widget.SetParent(nullptr);

		m_widgets.erase(found);
	}
}

void AmberEngine::UI::WidgetContainer::RemoveAllWidgets()
{
	m_widgets.clear();
}

void AmberEngine::UI::WidgetContainer::TransferOwnership(Widgets::AWidget& p_widget, WidgetContainer& p_widgetCoontainer)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::unique_ptr<Widgets::AWidget>& p_instance)
	{
		return p_instance.get() == &p_widget;
	});

	if (found != m_widgets.end())
	{
		p_widget.SetParent(&p_widgetCoontainer);

		p_widgetCoontainer.m_widgets.push_back(std::move(*found));

		m_widgets.erase(found);
	}
}

void AmberEngine::UI::WidgetContainer::CollectGarbage()
{
	m_widgets.erase(std::remove_if(m_widgets.begin(), m_widgets.end(), [](std::unique_ptr<Widgets::AWidget>& p_instance)
	{
		return p_instance && p_instance->IsDestroyed();
	}), m_widgets.end());
}

void AmberEngine::UI::WidgetContainer::DrawWidgets()
{
	CollectGarbage();

	if (m_isReversedDrawOrder)
	{
		for (auto it = m_widgets.crbegin(); it != m_widgets.crend(); ++it)
			it->get()->Draw();
	}
	else
	{
		for (const auto& widget : m_widgets)
			widget->Draw();
	}
}

void AmberEngine::UI::WidgetContainer::SetReverseDrawOrder(bool p_value)
{
	m_isReversedDrawOrder = p_value;
}

std::vector<std::unique_ptr<AmberEngine::UI::Widgets::AWidget>>& AmberEngine::UI::WidgetContainer::GetWidgets()
{
	return m_widgets;
}
