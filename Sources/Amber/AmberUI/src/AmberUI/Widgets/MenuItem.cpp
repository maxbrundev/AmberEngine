#include "Amberpch.h"

#include "AmberUI/Widgets/MenuItem.h"

AmberUI::Widgets::MenuItem::MenuItem(const std::string& p_name, const std::string& p_shortcut, bool p_checkable, bool p_checked) : DataWidget(Checked), Name(p_name), Shortcut(p_shortcut), Checkable(p_checkable), Checked(p_checked)
{
}

void AmberUI::Widgets::MenuItem::DrawImplementation()
{
	bool previousValue = Checked;

	if (ImGui::MenuItem((Name + m_widgetID).c_str(), Shortcut.c_str(), Checkable ? &Checked : nullptr, Enabled))
		ClickedEvent.Invoke();

	if (Checked != previousValue)
	{
		NotifyChange();
	}

	DrawContextualMenu();
	DrawWidgets();
}
