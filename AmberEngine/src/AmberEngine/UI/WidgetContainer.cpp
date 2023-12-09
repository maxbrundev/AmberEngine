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

void AmberEngine::UI::WidgetContainer::CollectGarbages()
{
	m_widgets.erase(std::remove_if(m_widgets.begin(), m_widgets.end(), [](std::unique_ptr<Widgets::AWidget>& p_instance)
	{
		return p_instance && p_instance->IsDestroyed();
	}), m_widgets.end());
}

void AmberEngine::UI::WidgetContainer::DrawWidgets()
{
	CollectGarbages();

	for (const auto& widget : m_widgets)
	{
		widget->Draw();
	}
}

std::vector<std::unique_ptr<AmberEngine::UI::Widgets::AWidget>>& AmberEngine::UI::WidgetContainer::GetWidgets()
{
	return m_widgets;
}
