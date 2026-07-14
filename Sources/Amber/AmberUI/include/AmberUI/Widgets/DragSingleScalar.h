#pragma once

#include "AmberUI/Widgets/DataWidget.h"

#include "AmberTools/Eventing/Event.h"

namespace AmberUI::Widgets
{
	template <typename T>
	class DragSingleScalar : public DataWidget<T>
	{
		static_assert(std::is_scalar<T>::value, "Invalid DragSingleScalar T (Scalar expected)");

	public:
		DragSingleScalar
		(
			ImGuiDataType p_dataType,
			T p_min,
			T p_max,
			T p_value,
			float p_speed,
			const std::string& p_label,
			const std::string& p_format
		) : DataWidget<T>(Value), Min(p_min), Max(p_max), Value(p_value), Speed(p_speed), Label(p_label), Format(p_format), m_dataType(p_dataType) {}

		virtual ~DragSingleScalar() override = default;

	protected:
		void DrawImplementation() override
		{
			if (Max < Min)
				Max = Min;

			if (Value < Min)
				Value = Min;
			else if (Value > Max)
				Value = Max;

			if (ImGui::DragScalar((Label + this->m_widgetID).c_str(), m_dataType, &Value, Speed, &Min, &Max, Format.c_str()))
			{
				this->NotifyChange();
			}
		}

	public:
		T Min;
		T Max;
		T Value;
		float Speed;
		std::string Label;
		std::string Format;

	private:
		ImGuiDataType m_dataType;
	};
}