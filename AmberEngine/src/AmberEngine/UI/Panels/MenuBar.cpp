#include "Amberpch.h"

#include "AmberEngine/UI/Panels/MenuBar.h"

#include "AmberEngine/Core/Context.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"

#include "AmberEngine/UI/Widgets/Separator.h"

AmberEngine::UI::Panels::MenuBar::MenuBar()
{
	CreateFileMenu();
	CreateWindowMenu();
}

void AmberEngine::UI::Panels::MenuBar::CreateFileMenu()
{
	auto& fileMenu = CreateWidget<Widgets::MenuList>("File");
	fileMenu.CreateWidget<Widgets::MenuItem>("Exit", "ALT + F4").ClickedEvent += [] { AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().window->SetShouldClose(true); };
}

void AmberEngine::UI::Panels::MenuBar::CreateWindowMenu()
{
	m_windowMenu = &CreateWidget<Widgets::MenuList>("Window");
	m_windowMenu->CreateWidget<Widgets::MenuItem>("Close all").ClickedEvent += std::bind(&AmberEngine::UI::Panels::MenuBar::OpenEveryWindows, this, false);
	m_windowMenu->CreateWidget<Widgets::MenuItem>("Open all").ClickedEvent += std::bind(&AmberEngine::UI::Panels::MenuBar::OpenEveryWindows, this, true);
	m_windowMenu->CreateWidget<Widgets::Separator>();
	
	/* When the menu is opened, we update which window is marked as "Opened" or "Closed" */
	m_windowMenu->ClickedEvent += std::bind(&AmberEngine::UI::Panels::MenuBar::UpdateToggleableItems, this);
}

void AmberEngine::UI::Panels::MenuBar::CreateActorsMenu()
{
	auto& actorsMenu = CreateWidget<Widgets::MenuList>("Actors");
}

void AmberEngine::UI::Panels::MenuBar::UpdateToggleableItems()
{
	for (auto&[name, panel] : m_panels)
		panel.second.get().checked = panel.first.get().IsOpened();
}

void AmberEngine::UI::Panels::MenuBar::OpenEveryWindows(bool p_state)
{
	for (auto&[name, panel] : m_panels)
		panel.first.get().SetOpened(p_state);
}

void AmberEngine::UI::Panels::MenuBar::RegisterPanel(const std::string& p_name, AmberEngine::UI::Panels::APanelWindow& p_panel)
{
	auto& menuItem = m_windowMenu->CreateWidget<Widgets::MenuItem>(p_name, "", true, true);
	menuItem.ValueChangedEvent += std::bind(&AmberEngine::UI::Panels::APanelWindow::SetOpened, &p_panel, std::placeholders::_1);
	
	m_panels.emplace(p_name, std::make_pair(std::ref(p_panel), std::ref(menuItem)));
}