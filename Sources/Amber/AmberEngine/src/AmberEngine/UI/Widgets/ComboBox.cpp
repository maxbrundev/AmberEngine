#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/ComboBox.h"

AmberEngine::UI::Widgets::ComboBox::ComboBox(int p_currentChoice) : DataWidget<int>(currentChoice), currentChoice(p_currentChoice)
{

}

void AmberEngine::UI::Widgets::ComboBox::DrawImplementation()
{
	if (choices.find(currentChoice) == choices.end())
		currentChoice = choices.begin()->first;

	if (ImGui::BeginCombo(m_widgetID.c_str(), choices[currentChoice].c_str()))
	{
		for (const auto&[key, value] : choices)
		{
			bool selected = key == currentChoice;

			if (ImGui::Selectable(value.c_str(), selected))
			{
				if (!selected)
				{
					ImGui::SetItemDefaultFocus();
					currentChoice = key;
					ValueChangedEvent.Invoke(currentChoice);
					NotifyChange();
				}
			}
		}

		ImGui::EndCombo();
	}
}
