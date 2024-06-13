#pragma once

#include "AmberEngine/UI/Widgets/AWidget.h"

namespace AmberEngine::UI::Widgets
{
	template <typename T>
	class DataWidget : public AWidget
	{
	public:
		DataWidget(T& p_dataHolder) : m_data(p_dataHolder) {}
		virtual ~DataWidget() override = default;

		inline void SetGathererAndProvider(std::function<T(void)> p_gatherer, std::function<void(T)> p_provider)
		{
			m_gatherer = p_gatherer;
			m_provider = p_provider;
		}

		inline virtual void Draw() override
		{
			if (enabled)
			{
				if (m_gatherer != nullptr)
					m_data = m_gatherer();

				AWidget::Draw();

				if (m_provider != nullptr)
					m_provider(m_data);
			}
		}

	private:
		T& m_data;
		std::function<T(void)> m_gatherer;
		std::function<void(T)> m_provider;
	};
}