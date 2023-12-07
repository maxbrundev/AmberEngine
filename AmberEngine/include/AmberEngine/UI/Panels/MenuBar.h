#pragma once

#include "AmberEngine/UI/Panels/PanelMenuBar.h"
#include "AmberEngine/UI/Panels/APanelWindow.h"

#include "AmberEngine/UI/Widgets/MenuList.h"
#include "AmberEngine/UI/Widgets/MenuItem.h"

namespace AmberEngine::UI::Panels
{
	class MenuBar : public PanelMenuBar
	{
		using PanelMap = std::unordered_map<std::string, std::pair<std::reference_wrapper<APanelWindow>, std::reference_wrapper<Widgets::MenuItem>>>;

	public:
		MenuBar();
		virtual ~MenuBar() override = default;

		void RegisterPanel(const std::string& p_name, APanelWindow& p_panel);

	private:
		void CreateFileMenu();
		void CreateWindowMenu();
		void CreateActorsMenu();

		void UpdateToggleableItems();
		void OpenEveryWindows(bool p_state);

	private:
		PanelMap m_panels;
		std::vector<Widgets::MenuList*> m_lists;
		Widgets::MenuList* m_windowMenu = nullptr;
	};
}
