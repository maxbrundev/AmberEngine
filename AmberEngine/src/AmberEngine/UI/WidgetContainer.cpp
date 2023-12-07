#include "Amberpch.h"

#include "AmberEngine/UI/WidgetContainer.h"

AmberEngine::UI::WidgetContainer::~WidgetContainer()
{
	RemoveAllWidgets();
}

void AmberEngine::UI::WidgetContainer::RemoveWidget(AWidget& p_widget)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::pair<AWidget*, EMemoryMode>& p_pair)
	{
		return p_pair.first == &p_widget;
	});

	if (found != m_widgets.end())
	{
		if (found->second == EMemoryMode::INTERNAL_MANAGMENT)
			delete found->first;

		m_widgets.erase(found);
	}
}

void AmberEngine::UI::WidgetContainer::RemoveAllWidgets()
{
	std::for_each(m_widgets.begin(), m_widgets.end(), [](auto& pair)
	{
		if (pair.second == EMemoryMode::INTERNAL_MANAGMENT)
			delete pair.first;
	});

	m_widgets.clear();
}

void AmberEngine::UI::WidgetContainer::ConsiderWidget(AWidget& p_widget, bool p_manageMemory)
{
	m_widgets.emplace_back(std::make_pair(&p_widget, p_manageMemory ? EMemoryMode::INTERNAL_MANAGMENT : EMemoryMode::EXTERNAL_MANAGMENT));
	p_widget.SetParent(this);
}

void AmberEngine::UI::WidgetContainer::UnconsiderWidget(AWidget& p_widget)
{
	auto found = std::find_if(m_widgets.begin(), m_widgets.end(), [&p_widget](std::pair<AWidget*, EMemoryMode>& p_pair)
	{
		return p_pair.first == &p_widget;
	});

	if (found != m_widgets.end())
	{
		p_widget.SetParent(nullptr);
		m_widgets.erase(found);
	}
}

void AmberEngine::UI::WidgetContainer::CollectGarbages()
{
	m_widgets.erase(std::remove_if(m_widgets.begin(), m_widgets.end(), [](std::pair<AWidget*, EMemoryMode>& p_item)
	{
		bool toDestroy = p_item.first && p_item.first->IsDestroyed();

		if (toDestroy && p_item.second == EMemoryMode::INTERNAL_MANAGMENT)
			delete p_item.first;

		return toDestroy;
	}), m_widgets.end());
}

void AmberEngine::UI::WidgetContainer::DrawWidgets()
{
	CollectGarbages();

	for (const auto& widget : m_widgets)
	{
		widget.first->Draw();
	}
}

std::vector<std::pair<AmberEngine::UI::AWidget*, AmberEngine::UI::EMemoryMode>>& AmberEngine::UI::WidgetContainer::GetWidgets()
{
	return m_widgets;
}
