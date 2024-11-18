#pragma once

#include "AmberEngine/UI/Widgets/DataWidget.h"

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::UI::Widgets
{
	template <typename T, size_t _Size>
	class DragMultipleScalars : public Widgets::DataWidget<std::array<T, _Size>>
	{
		static_assert(_Size > 1, "Invalid DragMultipleScalars _Size (2 or more requiered)");

	public:
		DragMultipleScalars
		(
			ImGuiDataType_ p_dataType,
			T p_min,
			T p_max,
			T p_value,
			float p_speed,
			const std::string& p_label,
			const std::string& p_format
		) : DataWidget<std::array<T, _Size>>(values), min(p_min), max(p_max), speed(p_speed), label(p_label), format(p_format), m_dataType(p_dataType)
		{
			values.fill(p_value);
		}

		virtual ~DragMultipleScalars() override = default;

	protected:
		void DrawImplementation() override
		{
			if (max < min)
				max = min;

			for (size_t i = 0; i < _Size; ++i)
			{
				if (values[i] < min)
					values[i] = min;
				else if (values[i] > max)
					values[i] = max;
			}

			if (ImGui::DragScalarN((label + this->m_widgetID).c_str(), m_dataType, values.data(), _Size, speed, &min, &max, format.c_str()))
			{
				ValueChangedEvent.Invoke(values);
				this->NotifyChange();
			}
		}

	public:
		T min;
		T max;
		float speed;
		std::array<T, _Size> values;
		std::string label;
		std::string format;
		Eventing::Event<std::array<T, _Size>&> ValueChangedEvent;

	protected:
		ImGuiDataType_ m_dataType;
	};
}