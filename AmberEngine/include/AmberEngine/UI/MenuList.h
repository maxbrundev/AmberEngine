#pragma once

#include "MenuItem.h"

class MenuList
{
public:
	MenuList(const std::string& p_name, bool p_locked = false);
	~MenuList();

	template <typename ... Args>
	MenuItem& AddItem(Args&&... p_args)
	{
		m_items.emplace_back(new MenuItem(p_args...));
		return *m_items.back();
	}

	void Draw();

public:
	std::string name;
	bool locked;
	AmberEngine::Eventing::Event<> ClickedEvent;

private:
	bool m_opened;

	std::vector<MenuItem*> m_items;
};
