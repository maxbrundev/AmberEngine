#pragma once

#include "AmberEngine/Eventing/Event.h"

class MenuItem
{
public:
	MenuItem(const std::string& p_name, const std::string& p_shortcut = "", bool p_checkable = false, bool p_checked = false);
	~MenuItem();

	void Draw();

public:
	std::string name;
	std::string shortcut;
	bool checkable;
	bool checked;
	AmberEngine::Eventing::Event<> ClickedEvent;
	AmberEngine::Eventing::Event<bool> ValueChangedEvent;
};
