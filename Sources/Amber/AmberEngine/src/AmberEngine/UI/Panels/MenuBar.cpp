#include "Amberpch.h"

#include "AmberEngine/UI/Panels/MenuBar.h"

#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Core/EditorAction.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/UI/Widgets/ContextualMenuItem.h"
#include "AmberEngine/UI/Widgets/InputText.h"

#include "AmberEngine/UI/Widgets/Separator.h"

#include "AmberEngine/Core/ActorCreationMenu.h"
#include "AmberEngine/Tools/Utils/String.h"
#include "AmberEngine/Core/EditorAction.h"

AmberEngine::UI::Panels::MenuBar::MenuBar()
{
	manager = &Tools::Global::ServiceLocator::Get<Core::UIManager>();
	CreateFileMenu();
	CreateWindowMenu();
	CreateActorsMenu();
	CreateResourcesMenu();
	CreateLayoutMenu();
}

void AmberEngine::UI::Panels::MenuBar::CreateFileMenu()
{
	auto& fileMenu = CreateWidget<Widgets::MenuList>("File");
	fileMenu.CreateWidget<Widgets::MenuItem>("New Scene", "CTRL + N").ClickedEvent += EDITOR_BIND(LoadEmptyScene);
	fileMenu.CreateWidget<Widgets::MenuItem>("Save Scene", "CTRL + S").ClickedEvent += EDITOR_BIND(SaveSceneChanges);
	fileMenu.CreateWidget<Widgets::MenuItem>("Save Scene As...", "CTRL + SHIFT + S").ClickedEvent += EDITOR_BIND(SaveAs);
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
	Utils::ActorCreationMenu::GenerateActorCreationMenu(actorsMenu);
}

void AmberEngine::UI::Panels::MenuBar::CreateLayoutMenu()
{
	std::filesystem::path dirPath = std::string(getenv("APPDATA")) + "\\AmberEngine\\Editor\\";

	m_layoutsPath = dirPath.string();

	auto& layoutMenuList = CreateWidget<Widgets::MenuList>("Layout");

	auto& saveMenuItem = layoutMenuList.CreateWidget<Widgets::MenuItem>("Save");
	saveMenuItem.ClickedEvent += EDITOR_BIND(SaveCurrentLayout);

	auto& saveNewMenuList = layoutMenuList.CreateWidget<Widgets::MenuList>("Save New");
	auto& layoutInputText = saveNewMenuList.CreateWidget<Widgets::InputText>("Layout Name");
	layoutInputText.selectAllOnClick = true;
	layoutInputText.EnterPressedEvent += [this](std::string basic_string)
	{
		if (basic_string.empty())
			return;

		EDITOR_EXEC(DelayAction(std::bind(&Core::UIManager::SaveLayout, manager, m_layoutsPath + basic_string + ".ini"), 1));
	};

	auto& loadMenuList = layoutMenuList.CreateWidget<Widgets::MenuList>("Load");

	loadMenuList.ClickedEvent += [&]
	{
		loadMenuList.m_widgets.clear();

		for (const auto& entry : std::filesystem::directory_iterator(m_layoutsPath))
		{
			if (entry.is_regular_file() && entry.path().extension() == ".ini")
			{
				std::shared_ptr<std::string> layoutFileName = std::make_shared<std::string>(entry.path().filename().string());

				auto& layoutMenuItem = loadMenuList.CreateWidget<Widgets::MenuItem>(*layoutFileName);
				layoutMenuItem.name = Tools::Utils::String::RemoveExtensionFromFileName(*layoutFileName);

				layoutMenuItem.ClickedEvent += [this, layoutFileName]
				{
					EDITOR_EXEC(DelayAction(std::bind(&Core::UIManager::SetLayout, manager, m_layoutsPath + *layoutFileName), 1));
				};

				auto& contextualMenu = layoutMenuItem.CreateContextualMenu<Widgets::ContextualMenuItem>();
				auto& deleteMenuItem = contextualMenu.CreateWidget<Widgets::MenuItem>("Delete");

				deleteMenuItem.ClickedEvent += [this, layoutFileName, &layoutMenuItem]
				{
					//EDITOR_EXEC(ResetToDefaultLayout);
					EDITOR_EXEC(DelayAction(std::bind(&Core::UIManager::DeleteLayout, manager, m_layoutsPath + *layoutFileName), 1));
					layoutMenuItem.enabled = false;
					
				};
				auto& renameToMenuList = contextualMenu.CreateWidget<Widgets::MenuList>("Rename to...");

				auto& renameInputText = renameToMenuList.CreateWidget<Widgets::InputText>("");
				renameInputText.content = Tools::Utils::String::RemoveExtensionFromFileName(*layoutFileName);
				renameInputText.selectAllOnClick = true;

				renameInputText.EnterPressedEvent += [this, layoutFileName, &contextualMenu, &layoutMenuItem](std::string p_newName)
				{
					if (p_newName.empty())
						return;

					layoutMenuItem.name = p_newName;
					//EDITOR_EXEC(ResetToDefaultLayout);
					std::string oldFileName = m_layoutsPath + *layoutFileName;
					std::string newFileName = m_layoutsPath + p_newName + ".ini";
					EDITOR_EXEC(DelayAction(std::bind(&Core::UIManager::RenameLayout, manager, oldFileName, newFileName), 1));

					*layoutFileName = p_newName + ".ini";
					contextualMenu.Close();
				};
			}
		}
	};

	layoutMenuList.CreateWidget<Widgets::MenuItem>("Reset").ClickedEvent += EDITOR_BIND(ResetToDefaultLayout);
}

void AmberEngine::UI::Panels::MenuBar::CreateResourcesMenu()
{
	auto& resourcesMenu = CreateWidget<Widgets::MenuList>("Resources");
	resourcesMenu.CreateWidget<Widgets::MenuItem>("Compile shaders").ClickedEvent += EDITOR_BIND(CompileShaders);
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