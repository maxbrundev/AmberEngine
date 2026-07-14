#include "Amberpch.h"

#include "AmberUI/WidgetContainer.h"

AmberUI::WidgetContainer::~WidgetContainer()
{
	RemoveAllWidgets();
}

void AmberUI::WidgetContainer::RemoveWidget(Widgets::AWidget& p_widget)
{
	if (m_isDrawing)
	{
		p_widget.Destroy();
		return;
	}

	auto removeFrom = [&p_widget](std::vector<std::unique_ptr<Widgets::AWidget>>& p_widgets)
	{
		auto found = std::find_if(p_widgets.begin(), p_widgets.end(), [&p_widget](std::unique_ptr<Widgets::AWidget>& p_instance)
		{
			return p_instance.get() == &p_widget;
		});

		if (found != p_widgets.end())
		{
			p_widget.SetParent(nullptr);

			p_widgets.erase(found);

			return true;
		}

		return false;
	};

	if (!removeFrom(m_widgets))
	{
		removeFrom(m_pendingWidgets);
	}
}

void AmberUI::WidgetContainer::RemoveAllWidgets()
{
	if (m_isDrawing)
	{
		for (auto& widget : m_widgets)
		{
			widget->Destroy();
		}

		for (auto& widget : m_pendingWidgets)
		{
			widget->Destroy();
		}

		return;
	}

	m_widgets.clear();
	m_pendingWidgets.clear();
}

void AmberUI::WidgetContainer::TransferWidget(Widgets::AWidget& p_widget, WidgetContainer& p_destination)
{
	WidgetContainer* source = p_widget.GetParent();

	if (!source || source == &p_destination)
	{
		return;
	}

	if (source->m_isDrawing)
	{
		source->m_pendingOperations.push_back([&p_widget, &p_destination] { TransferWidget(p_widget, p_destination); });
		return;
	}

	auto extract = [&p_widget](std::vector<std::unique_ptr<Widgets::AWidget>>& p_widgets)
	{
		std::unique_ptr<Widgets::AWidget> result;

		auto found = std::find_if(p_widgets.begin(), p_widgets.end(), [&p_widget](std::unique_ptr<Widgets::AWidget>& p_instance)
		{
			return p_instance.get() == &p_widget;
		});

		if (found != p_widgets.end())
		{
			result = std::move(*found);
			p_widgets.erase(found);
		}

		return result;
	};

	std::unique_ptr<Widgets::AWidget> widget = extract(source->m_widgets);

	if (!widget)
	{
		widget = extract(source->m_pendingWidgets);
	}

	if (widget)
	{
		p_widget.SetParent(&p_destination);
		p_destination.AddWidget(std::move(widget));
	}
}

void AmberUI::WidgetContainer::CollectGarbage()
{
	m_widgets.erase(std::remove_if(m_widgets.begin(), m_widgets.end(), [](std::unique_ptr<Widgets::AWidget>& p_instance)
	{
		return p_instance && p_instance->IsDestroyed();
	}), m_widgets.end());
}

void AmberUI::WidgetContainer::DrawWidgets()
{
	FlushPendingWidgets();
	CollectGarbage();

	m_isDrawing = true;

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

	m_isDrawing = false;

	FlushPendingWidgets();
	FlushPendingOperations();
	FlushPendingWidgets();
	CollectGarbage();
}

void AmberUI::WidgetContainer::SetReverseDrawOrder(bool p_value)
{
	m_isReversedDrawOrder = p_value;
}

void AmberUI::WidgetContainer::MoveWidgetIndex(size_t p_fromIndex, size_t p_toIndex)
{
	if (m_isDrawing)
	{
		m_pendingOperations.push_back([this, p_fromIndex, p_toIndex] { MoveWidgetIndex(p_fromIndex, p_toIndex); });
		return;
	}

	if (p_fromIndex > m_widgets.size() - 1 || p_toIndex > m_widgets.size() - 1)
	{
		return;
	}

	if (p_fromIndex == p_toIndex)
	{
		return;
	}

	if (p_fromIndex < p_toIndex)
	{
		std::rotate(m_widgets.begin() + p_fromIndex, m_widgets.begin() + p_fromIndex + 1, m_widgets.begin() + p_toIndex + 1);
	}
	else
	{
		std::rotate(m_widgets.begin() + p_toIndex, m_widgets.begin() + p_fromIndex, m_widgets.begin() + p_fromIndex + 1);
	}
}

std::vector<std::unique_ptr<AmberUI::Widgets::AWidget>>& AmberUI::WidgetContainer::GetWidgets()
{
	return m_widgets;
}

void AmberUI::WidgetContainer::AddWidget(std::unique_ptr<Widgets::AWidget>&& p_widget)
{
	if (m_isDrawing)
	{
		m_pendingWidgets.push_back(std::move(p_widget));
	}
	else
	{
		m_widgets.push_back(std::move(p_widget));
	}
}

void AmberUI::WidgetContainer::FlushPendingWidgets()
{
	for (auto& pendingWidget : m_pendingWidgets)
	{
		m_widgets.push_back(std::move(pendingWidget));
	}

	m_pendingWidgets.clear();
}

void AmberUI::WidgetContainer::FlushPendingOperations()
{
	std::vector<std::function<void()>> pendingOperations = std::move(m_pendingOperations);
	m_pendingOperations.clear();

	for (auto& pendingOperation : pendingOperations)
	{
		pendingOperation();
	}
}