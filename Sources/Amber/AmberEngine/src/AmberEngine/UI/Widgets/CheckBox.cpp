#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/CheckBox.h"

AmberEngine::UI::Widgets::CheckBox::CheckBox(bool p_value, const std::string& p_label) : DataWidget<bool>(value), value(p_value), label(p_label)
{

}

void AmberEngine::UI::Widgets::CheckBox::DrawImplementation()
{
	bool previousValue = value;

	ImGui::Checkbox((label + m_widgetID).c_str(), &value);

	if (value != previousValue)
	{
		ValueChangedEvent.Invoke(value);
		NotifyChange();
	}
}
