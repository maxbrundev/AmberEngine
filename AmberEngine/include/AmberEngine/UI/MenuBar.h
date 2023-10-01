#pragma once

#include "AmberEngine/UI/Panels/APanel.h"
#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/UI/MenuList.h"

class MenuBar : public AmberEngine::UI::APanel
{
	using PanelMap = std::unordered_map<std::string, std::pair<std::reference_wrapper<AmberEngine::UI::APanelWindow>, std::reference_wrapper<MenuItem>>>;

public:
	MenuBar();
	~MenuBar() override;

	template <typename ... Args>
	MenuList& AddList(Args&&... p_args)
	{
		m_lists.emplace_back(new MenuList(p_args...));

		return *m_lists.back();
	}

	void RegisterPanel(const std::string& p_name, AmberEngine::UI::APanelWindow& p_panel);

protected:
	void DrawImplementation() override;

private:
	void CreateFileMenu();
	void CreateWindowMenu();

	void UpdateToggleableItems();

private:
	PanelMap m_panels;
	std::vector<MenuList*> m_lists;
	MenuList* m_windowMenu = nullptr;
};
