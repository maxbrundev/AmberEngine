#pragma once


#include <map>

#include "AmberUI/Core/UIManager.h"
#include "AmberUI/Panels/PanelMenuBar.h"
#include "AmberUI/Panels/APanelWindow.h"
#include "AmberUI/Widgets/Button.h"

#include "AmberUI/Widgets/MenuList.h"
#include "AmberUI/Widgets/MenuItem.h"

namespace AmberEditor::Panels
{
	class MenuBar : public AmberUI::Panels::PanelMenuBar
	{
		using PanelMap = std::unordered_map<std::string, std::pair<std::reference_wrapper<AmberUI::Panels::APanelWindow>, std::reference_wrapper<AmberUI::Widgets::MenuItem>>>;

	public:
		MenuBar();
		virtual ~MenuBar() override = default;

		void RegisterPanel(const std::string& p_name, AmberUI::Panels::APanelWindow& p_panel);

	private:
		void CreateFileMenu();
		void CreateBuildMenu();
		void CreateWindowMenu();
		void CreateActorsMenu();
		void CreateLayoutMenu();
		void CreateResourcesMenu();
		void CreateSettingsMenu();

		void UpdateToggleableItems();
		void OpenEveryWindows(bool p_state);

	private:
		PanelMap m_panels;
		std::vector<AmberUI::Widgets::MenuList*> m_lists;
		AmberUI::Widgets::MenuList* m_windowMenu = nullptr;
		AmberUI::Core::UIManager* manager;
		std::string m_layoutsPath;
	};
}
