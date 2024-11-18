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
		void SetReverseDrawOrder(bool p_value = true);

		template <typename T, typename ... Args>
		T& CreateWidget(Args&&... p_args)
		{
			m_widgets.emplace_back(std::make_unique<T>(p_args...));
			T& instance = *static_cast<T*>(m_widgets.back().get());
			instance.SetParent(this);
			return instance;
		}

		void MoveWidgetIndex(size_t p_fromIndex, size_t p_toIndex)
		{
			if (p_fromIndex > m_widgets.size() - 1 || p_toIndex > m_widgets.size() - 1) 
			{
				return;
			}

			if (p_fromIndex == p_toIndex) {
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

		std::vector<std::unique_ptr<Widgets::AWidget>>& GetWidgets();

	public:
		std::vector<std::unique_ptr<Widgets::AWidget>> m_widgets;

		bool m_isReversedDrawOrder = false;
	};
}
