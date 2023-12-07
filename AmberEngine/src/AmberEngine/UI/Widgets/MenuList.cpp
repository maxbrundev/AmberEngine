#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/MenuList.h"

AmberEngine::UI::Widgets::MenuList::MenuList(const std::string& p_name, bool p_locked) :
name(p_name), locked(p_locked)
{
}

void AmberEngine::UI::Widgets::MenuList::DrawImplementation()
{
	if (ImGui::BeginMenu(name.c_str(), !locked))
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