#pragma once


#include <map>

#include "AmberEngine/UI/Core/UIManager.h"
#include "AmberEngine/UI/Panels/PanelMenuBar.h"
#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/UI/Widgets/Button.h"

#include "AmberEngine/UI/Widgets/MenuList.h"
#include "AmberEngine/UI/Widgets/MenuItem.h"

namespace AmberEngine::UI::Panels
{
	class MenuBar : public PanelMenuBar
	{
		using PanelMap = std::unordered_map<std::string, std::pair<std::reference_wrapper<AmberEngine::UI::Panels::APanelWindow>, std::reference_wrapper<AmberEngine::UI::Widgets::MenuItem>>>;

	public:
		MenuBar();
		virtual ~MenuBar() override = default;

		void RegisterPanel(const std::string& p_name, AmberEngine::UI::Panels::APanelWindow& p_panel);

	private:
		void CreateFileMenu();
		void CreateWindowMenu();
		void CreateActorsMenu();
		void CreateLayoutMenu();
		void CreateResourcesMenu();

		void UpdateToggleableItems();
		void OpenEveryWindows(bool p_state);

	private:
		PanelMap m_panels;
		std::vector<Widgets::MenuList*> m_lists;
		Widgets::MenuList* m_windowMenu = nullptr;
		Core::UIManager* manager;
		std::string m_layoutsPath;
	};
}
