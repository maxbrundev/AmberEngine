#pragma once

#include "AmberUI/Widgets/AWidget.h"

namespace AmberUI
{
	class WidgetContainer
	{
	public:
		WidgetContainer() = default;
		virtual ~WidgetContainer();

		void RemoveWidget(Widgets::AWidget& p_widget);
		void RemoveAllWidgets();
		static void TransferWidget(Widgets::AWidget& p_widget, WidgetContainer& p_destination);
		void CollectGarbage();
		void DrawWidgets();
		void SetReverseDrawOrder(bool p_value = true);

		template <typename T, typename ... Args>
		T& CreateWidget(Args&&... p_args)
		{
			auto& destination = m_isDrawing ? m_pendingWidgets : m_widgets;
			destination.emplace_back(std::make_unique<T>(p_args...));
			T& instance = *static_cast<T*>(destination.back().get());
			instance.SetParent(this);
			return instance;
		}

		void MoveWidgetIndex(size_t p_fromIndex, size_t p_toIndex);

		std::vector<std::unique_ptr<Widgets::AWidget>>& GetWidgets();

	private:
		void AddWidget(std::unique_ptr<Widgets::AWidget>&& p_widget);
		void FlushPendingWidgets();
		void FlushPendingOperations();

	public:
		std::vector<std::unique_ptr<Widgets::AWidget>> m_widgets;

		bool m_isReversedDrawOrder = false;

	private:
		std::vector<std::unique_ptr<Widgets::AWidget>> m_pendingWidgets;
		std::vector<std::function<void()>> m_pendingOperations;

		bool m_isDrawing = false;
	};
}
