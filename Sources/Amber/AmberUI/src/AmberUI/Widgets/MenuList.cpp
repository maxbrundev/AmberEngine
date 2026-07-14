#include "Amberpch.h"

#include "AmberUI/Widgets/MenuList.h"

AmberUI::Widgets::MenuList::MenuList(const std::string& p_name, bool p_locked) :
Name(p_name), Locked(p_locked)
{
}

void AmberUI::Widgets::MenuList::DrawImplementation()
{
	if (ImGui::BeginMenu(Name.c_str(), !Locked))
	{
		if (!m_opened)
		{
			ClickedEvent.Invoke();
			m_opened = true;
		}

		DrawWidgets();
		ImGui::EndMenu();
	}
	else
	{
		m_opened = false;
	}
}