#pragma once

#include "AmberEngine/UI/Widgets/AWidget.h"

namespace AmberEngine::UI
{
	class WidgetContainer
	{
	public:
		WidgetContainer() = default;
		virtual ~WidgetContainer();

		void RemoveWidget(Widgets::AWidget& p_widget);
		void RemoveAllWidgets();
		void TransferOwnership(Widgets::AWidget& p_widget, WidgetContainer& p_widgetCoontainer);
		void CollectGarbage();
		void DrawWidgets();

		template <typename T, typename ... Args>
		T& CreateWidget(Args&&... p_args)
		{
			m_widgets.emplace_back(std::make_unique<T>(p_args...));
			T& instance = *static_cast<T*>(m_widgets.back().get());
			instance.SetParent(this);
			return instance;
		}

		std::vector<std::unique_ptr<Widgets::AWidget>>& GetWidgets();

	public:
		std::vector<std::unique_ptr<Widgets::AWidget>> m_widgets;
	};
}
