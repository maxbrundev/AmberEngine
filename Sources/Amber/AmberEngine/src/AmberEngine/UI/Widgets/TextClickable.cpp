#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/TextClickable.h"

AmberEngine::UI::Widgets::TextClickable::TextClickable(const std::string& p_content) : Text(p_content)
{
	m_autoExecutePlugins = false;
}


void AmberEngine::UI::Widgets::TextClickable::DrawImplementation()
{
	bool useless = false;

	if (ImGui::Selectable((content + m_widgetID).c_str(), &useless, ImGuiSelectableFlags_AllowDoubleClick))
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

	ExecutePlugins();

	DrawWidgets();
}
