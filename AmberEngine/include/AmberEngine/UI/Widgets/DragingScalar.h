#pragma once

#include "AmberEngine/UI/Widgets/DataWidget.h"

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::UI::Widgets
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
		) : DataWidget<T>(value), min(p_min), max(p_max), value(p_value), speed(p_speed), label(p_label), format(p_format), m_dataType(p_dataType) {}

		virtual ~DragSingleScalar() override = default;

	protected:
		void DrawImplementation() override
		{
			if (max < min)
				max = min;

			if (value < min)
				value = min;
			else if (value > max)
				value = max;

			if (ImGui::DragScalar((label + this->m_widgetID).c_str(), m_dataType, &value, speed, &min, &max, format.c_str()))
			{
				ValueChangedEvent.Invoke(value);
			}
		}

	public:
		T min;
		T max;
		T value;
		float speed;
		std::string label;
		std::string format;
		Eventing::Event<T> ValueChangedEvent;

	private:
		ImGuiDataType m_dataType;
	};
}