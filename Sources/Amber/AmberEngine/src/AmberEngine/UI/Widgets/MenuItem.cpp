#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/MenuItem.h"

AmberEngine::UI::Widgets::MenuItem::MenuItem(const std::string& p_name, const std::string& p_shortcut, bool p_checkable, bool p_checked) : DataWidget(m_selected),
                                                                                                                                           name(p_name), shortcut(p_shortcut), checkable(p_checkable), checked(p_checked)
{
}

void AmberEngine::UI::Widgets::MenuItem::DrawImplementation()
{
	bool previousValue = checked;

	if (ImGui::MenuItem((name + m_widgetID).c_str(), shortcut.c_str(), checkable ? &checked : nullptr, enabled))
		ClickedEvent.Invoke();

	if (checked != previousValue)
	{
		ValueChangedEvent.Invoke(checked);
		NotifyChange();
	}

	DrawContextualMenu();
	DrawWidgets();
}