#pragma once

#include "AmberUI/Widgets/DataWidget.h"

#include "AmberTools/Eventing/Event.h"

namespace AmberUI::Widgets
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
		) : DataWidget<std::array<T, _Size>>(Values), Min(p_min), Max(p_max), Speed(p_speed), Label(p_label), Format(p_format), m_dataType(p_dataType)
		{
			Values.fill(p_value);
		}

		virtual ~DragMultipleScalars() override = default;

	protected:
		void DrawImplementation() override
		{
			if (Max < Min)
				Max = Min;

			for (size_t i = 0; i < _Size; ++i)
			{
				if (Values[i] < Min)
					Values[i] = Min;
				else if (Values[i] > Max)
					Values[i] = Max;
			}

			if (ImGui::DragScalarN((Label + this->m_widgetID).c_str(), m_dataType, Values.data(), _Size, Speed, &Min, &Max, Format.c_str()))
			{
				ValueChangedEvent.Invoke(Values);
				this->NotifyChange();
			}
		}

	public:
		T Min;
		T Max;
		float Speed;
		std::array<T, _Size> Values;
		std::string Label;
		std::string Format;
		AmberTools::Eventing::Event<std::array<T, _Size>&> ValueChangedEvent;

	protected:
		ImGuiDataType_ m_dataType;
	};
}