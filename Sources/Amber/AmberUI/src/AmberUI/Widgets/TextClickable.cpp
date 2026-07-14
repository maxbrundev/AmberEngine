#include "Amberpch.h"

#include "AmberUI/Widgets/TextClickable.h"

AmberUI::Widgets::TextClickable::TextClickable(const std::string& p_content) : Text(p_content)
{
	m_autoExecuteDragDrop = false;
}


void AmberUI::Widgets::TextClickable::DrawImplementation()
{
	bool useless = false;

	if (ImGui::Selectable((Content + m_widgetID).c_str(), &useless, ImGuiSelectableFlags_AllowDoubleClick))
	{
		if (ImGui::IsMouseDoubleClicked(0))
		{
			DoubleClickedEvent.Invoke();
		}
		else
		{
			ClickedEvent.Invoke();
		}
	}


	DrawContextualMenu();

	DrawnEvent.Invoke();

	DrawDragDrop();

	DrawWidgets();
}
