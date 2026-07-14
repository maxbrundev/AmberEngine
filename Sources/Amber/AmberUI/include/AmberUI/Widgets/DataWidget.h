#pragma once

#include "AmberUI/Widgets/AWidget.h"

namespace AmberUI::Widgets
{
	template <typename T>
	class DataWidget : public AWidget
	{
	public:
		DataWidget(T& p_dataHolder) : m_data(p_dataHolder)
		{
		}

		virtual ~DataWidget() override = default;

		void RegisterReference(T& p_reference)
		{
			m_dataPointer = &p_reference;
		}

		void SetGathererAndProvider(const std::function<T()>& p_gatherer, const std::function<void(T)>& p_provider)
		{
			m_gatherer = p_gatherer;
			m_provider = p_provider;
		}

		void NotifyChange()
		{
			m_valueChanged = true;
		}

		virtual void Draw() override
		{
			if (Enabled)
			{
				if (m_gatherer != nullptr)
				{
					m_data = m_dataPointer ? *m_dataPointer : m_gatherer();
				}

				AWidget::Draw();

				if (m_valueChanged)
				{
					if (m_provider != nullptr)
					{
						m_provider(m_data);
					}

					ValueChangedEvent.Invoke(m_data);
					m_valueChanged = false;
				}
			}
		}

	public:
		AmberTools::Eventing::Event<T> ValueChangedEvent;

	protected:
		T& m_data;

	private:
		T* m_dataPointer = nullptr;
		std::function<T()> m_gatherer = nullptr;
		std::function<void(T)> m_provider = nullptr;
		bool m_valueChanged = false;
	};
}
