#include "Amberpch.h"

#include "AmberUI/Widgets/CheckBox.h"

AmberUI::Widgets::CheckBox::CheckBox(bool p_value, const std::string& p_label) : DataWidget<bool>(Value), Value(p_value), Label(p_label)
{

}

void AmberUI::Widgets::CheckBox::DrawImplementation()
{
	bool previousValue = Value;

	ImGui::Checkbox((Label + m_widgetID).c_str(), &Value);

	if (Value != previousValue)
	{
		NotifyChange();
	}
}
