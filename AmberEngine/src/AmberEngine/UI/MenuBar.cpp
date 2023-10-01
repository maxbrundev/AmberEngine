#include "Amberpch.h"

#include "AmberEngine/UI/MenuBar.h"

#include "AmberEngine/Tools/Utils/ServiceLocator.h"
#include "AmberEngine/Core/Context.h"
#include "AmberEngine/UI/Panels/APanelWindow.h"

MenuBar::MenuBar()
{
	CreateFileMenu();
	CreateWindowMenu();
}

MenuBar::~MenuBar()
{
	for (auto& list: m_lists)
	{
		delete list;
		list = nullptr;
	}

	m_lists.clear();
}

void MenuBar::CreateFileMenu()
{
	auto& fileListe = AddList("File");
	fileListe.AddItem("Exit", "ALT + F4").ClickedEvent += [] { AmberEngine::Utils::ServiceLocator::Get<AmberEngine::Core::Context>().window->SetShouldClose(true); };
}

void MenuBar::CreateWindowMenu()
{
	m_windowMenu = &AddList("Window");

	m_windowMenu->ClickedEvent += std::bind(&MenuBar::UpdateToggleableItems, this);
}

void MenuBar::UpdateToggleableItems()
{
	for (auto&[name, panel] : m_panels)
		panel.second.get().checked = panel.first.get().IsOpened();
}

void MenuBar::RegisterPanel(const std::string& p_name, AmberEngine::UI::APanelWindow& p_panel)
{
	auto& menuItem = m_windowMenu->AddItem(p_name, "", true, true);
	menuItem.ValueChangedEvent += std::bind(&AmberEngine::UI::APanelWindow::SetOpened, &p_panel, std::placeholders::_1);

	m_panels.emplace(p_name, std::make_pair(std::ref(p_panel), std::ref(menuItem)));
}

void MenuBar::DrawImplementation()
{
	if (!m_lists.empty() && ImGui::BeginMainMenuBar())
	{
		for (auto menuList : m_lists)
		{
			menuList->Draw();
		}

		//m_windowMenu->Draw();

		ImGui::EndMainMenuBar();
	}
}
