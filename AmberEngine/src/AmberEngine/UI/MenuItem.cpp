#include "Amberpch.h"

#include "AmberEngine/UI/MenuItem.h"

MenuItem::MenuItem(const std::string& p_name, const std::string& p_shortcut, bool p_checkable, bool p_checked) :
name(p_name), shortcut(p_shortcut), checkable(p_checkable), checked(p_checked)
{
}

MenuItem::~MenuItem()
{
}

void MenuItem::Draw()
{
	bool previousValue = checked;

	if (ImGui::MenuItem((name).c_str(), shortcut.c_str(), checkable ? &checked : nullptr))
		ClickedEvent.Invoke();

	if (checked != previousValue)
	{
		ValueChangedEvent.Invoke(checked);
	}
}
