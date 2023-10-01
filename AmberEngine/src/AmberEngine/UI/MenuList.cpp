#include "Amberpch.h"

#include "AmberEngine/UI/MenuList.h"

MenuList::MenuList(const std::string& p_name, bool p_locked) :
name(p_name), locked(p_locked)
{
}

MenuList::~MenuList()
{
	for (auto& item : m_items)
	{
		delete item;
		item = nullptr;
	}

	m_items.clear();
}

void MenuList::Draw()
{
	if (ImGui::BeginMenu(name.c_str(), !locked))
	{
		if (!m_opened)
		{
			ClickedEvent.Invoke();
			m_opened = true;
		}

		for (auto item : m_items)
		{
			item->Draw();
		}

		ImGui::EndMenu();
	}
	else
	{
		m_opened = false;
	}
}
