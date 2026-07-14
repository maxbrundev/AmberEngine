#include "Amberpch.h"

#include "AmberUI/Widgets/ComboBox.h"

AmberUI::Widgets::ComboBox::ComboBox(int p_currentChoice) : DataWidget<int>(CurrentChoice), CurrentChoice(p_currentChoice)
{

}

void AmberUI::Widgets::ComboBox::DrawImplementation()
{
	if (Choices.find(CurrentChoice) == Choices.end())
		CurrentChoice = Choices.begin()->first;

	if (ImGui::BeginCombo(m_widgetID.c_str(), Choices[CurrentChoice].c_str()))
	{
		for (const auto&[key, value] : Choices)
		{
			bool selected = key == CurrentChoice;

			if (ImGui::Selectable(value.c_str(), selected))
			{
				if (!selected)
				{
					ImGui::SetItemDefaultFocus();
					CurrentChoice = key;
					NotifyChange();
				}
			}
		}

		ImGui::EndCombo();
	}
}
