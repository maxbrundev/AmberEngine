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

		void RegisterReference(T& p_reference)
		{
			m_dataPointer = &p_reference;
		}

		inline void SetGathererAndProvider(const std::function<T(void)>& p_gatherer, const std::function<void(T)>& p_provider)
		{
			m_gatherer = p_gatherer;
			m_provider = p_provider;
		}

		inline virtual void Draw() override
		{
			if (enabled)
			{
				if (m_gatherer != nullptr)
				{
					m_data = m_dataPointer ? *m_dataPointer : m_gatherer();
				}

				AWidget::Draw();

				if (m_valueChanged && m_provider != nullptr)
				{
					m_provider(m_data);
					m_valueChanged = false;
				}
			}
		}

		inline void NotifyChange()
		{
			m_valueChanged = true;
		}

	private:
		T& m_data;
		T* m_dataPointer = nullptr;
		std::function<T(void)> m_gatherer;
		std::function<void(T)> m_provider;
		bool m_valueChanged = false;
	};
}