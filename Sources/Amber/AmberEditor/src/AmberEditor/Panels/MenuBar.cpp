#include "Amberpch.h"

#include "AmberEditor/Panels/MenuBar.h"

#include "AmberEditor/Core/Context.h"
#include "AmberEditor/Core/EditorAction.h"

#include "AmberTools/Global/ServiceLocator.h"
#include "AmberUI/Widgets/ContextualMenuItem.h"
#include "AmberUI/Widgets/DragSingleScalar.h"
#include "AmberUI/Widgets/InputText.h"

#include "AmberUI/Widgets/Separator.h"

#include "AmberEditor/Settings/EditorSettings.h"
#include "AmberEditor/Utils/ActorCreationMenu.h"
#include "AmberTools/Utils/String.h"
#include "AmberEditor/Core/EditorAction.h"

AmberEditor::Panels::MenuBar::MenuBar()
{
	manager = &AmberTools::Global::ServiceLocator::Get<AmberUI::Core::UIManager>();
	CreateFileMenu();
	CreateBuildMenu();
	CreateWindowMenu();
	CreateActorsMenu();
	CreateResourcesMenu();
	CreateSettingsMenu();
	CreateLayoutMenu();
}

void AmberEditor::Panels::MenuBar::CreateFileMenu()
{
	auto& fileMenu = CreateWidget<AmberUI::Widgets::MenuList>("File");
	fileMenu.CreateWidget<AmberUI::Widgets::MenuItem>("New Scene", "CTRL + N").ClickedEvent += EDITOR_BIND(LoadEmptyScene);
	fileMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Save Scene", "CTRL + S").ClickedEvent += EDITOR_BIND(SaveSceneChanges);
	fileMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Save Scene As...", "CTRL + SHIFT + S").ClickedEvent += EDITOR_BIND(SaveAs);
	fileMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Exit", "ALT + F4").ClickedEvent += [] { AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::Context>().window->SetShouldClose(true); };
}

void AmberEditor::Panels::MenuBar::CreateBuildMenu()
{
	auto& buildMenu = CreateWidget<AmberUI::Widgets::MenuList>("Build");
	buildMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Build game").ClickedEvent += EDITOR_BIND(Build, false, false);
	buildMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Build game and run").ClickedEvent += EDITOR_BIND(Build, true, false);
	buildMenu.CreateWidget<AmberUI::Widgets::Separator>();
	buildMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Temporary build").ClickedEvent += EDITOR_BIND(Build, true, true);
}

void AmberEditor::Panels::MenuBar::CreateSettingsMenu()
{
	auto& settingsMenu = CreateWidget<AmberUI::Widgets::MenuList>("Settings");

	auto& snappingMenu = settingsMenu.CreateWidget<AmberUI::Widgets::MenuList>("Snapping");
	snappingMenu.CreateWidget<AmberUI::Widgets::DragSingleScalar<float>>(ImGuiDataType_Float, 0.001f, 999999.0f, Settings::EditorSettings::TranslationSnapUnit, 0.05f, "Translation Unit", "%.3f").ValueChangedEvent += [](float p_value) { Settings::EditorSettings::TranslationSnapUnit = p_value; };
	snappingMenu.CreateWidget<AmberUI::Widgets::DragSingleScalar<float>>(ImGuiDataType_Float, 0.001f, 999999.0f, Settings::EditorSettings::RotationSnapUnit, 1.0f, "Rotation Unit", "%.3f").ValueChangedEvent += [](float p_value) { Settings::EditorSettings::RotationSnapUnit = p_value; };
	snappingMenu.CreateWidget<AmberUI::Widgets::DragSingleScalar<float>>(ImGuiDataType_Float, 0.001f, 999999.0f, Settings::EditorSettings::ScalingSnapUnit, 0.05f, "Scaling Unit", "%.3f").ValueChangedEvent += [](float p_value) { Settings::EditorSettings::ScalingSnapUnit = p_value; };

	auto& debuggingMenu = settingsMenu.CreateWidget<AmberUI::Widgets::MenuList>("Debugging");
	debuggingMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Show geometry bounds", "", true, Settings::EditorSettings::ShowGeometryBounds).ValueChangedEvent += [](bool p_value) { Settings::EditorSettings::ShowGeometryBounds = p_value; };
	debuggingMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Show lights bounds", "", true, Settings::EditorSettings::ShowLightBounds).ValueChangedEvent += [](bool p_value) { Settings::EditorSettings::ShowLightBounds = p_value; };
}

void AmberEditor::Panels::MenuBar::CreateWindowMenu()
{
	m_windowMenu = &CreateWidget<AmberUI::Widgets::MenuList>("Window");
	m_windowMenu->CreateWidget<AmberUI::Widgets::MenuItem>("Close all").ClickedEvent += std::bind(&AmberEditor::Panels::MenuBar::OpenEveryWindows, this, false);
	m_windowMenu->CreateWidget<AmberUI::Widgets::MenuItem>("Open all").ClickedEvent += std::bind(&AmberEditor::Panels::MenuBar::OpenEveryWindows, this, true);
	m_windowMenu->CreateWidget<AmberUI::Widgets::Separator>();
	
	/* When the menu is opened, we update which window is marked as "Opened" or "Closed" */
	m_windowMenu->ClickedEvent += std::bind(&AmberEditor::Panels::MenuBar::UpdateToggleableItems, this);
}

void AmberEditor::Panels::MenuBar::CreateActorsMenu()
{
	auto& actorsMenu = CreateWidget<AmberUI::Widgets::MenuList>("Actors");
	AmberEditor::Utils::ActorCreationMenu::GenerateActorCreationMenu(actorsMenu);
}

void AmberEditor::Panels::MenuBar::CreateLayoutMenu()
{
	std::filesystem::path dirPath = std::string(getenv("APPDATA")) + "\\AmberEngine\\Editor\\";

	m_layoutsPath = dirPath.string();

	auto& layoutMenuList = CreateWidget<AmberUI::Widgets::MenuList>("Layout");

	auto& saveMenuItem = layoutMenuList.CreateWidget<AmberUI::Widgets::MenuItem>("Save");
	saveMenuItem.ClickedEvent += EDITOR_BIND(SaveCurrentLayout);

	auto& saveNewMenuList = layoutMenuList.CreateWidget<AmberUI::Widgets::MenuList>("Save New");
	auto& layoutInputText = saveNewMenuList.CreateWidget<AmberUI::Widgets::InputText>("Layout Name");
	layoutInputText.SelectAllOnClick = true;
	layoutInputText.EnterPressedEvent += [this](std::string basic_string)
	{
		if (basic_string.empty())
			return;

		EDITOR_EXEC(DelayAction(std::bind(&AmberUI::Core::UIManager::SaveLayout, manager, m_layoutsPath + basic_string + ".ini"), 1));
	};

	auto& loadMenuList = layoutMenuList.CreateWidget<AmberUI::Widgets::MenuList>("Load");

	loadMenuList.ClickedEvent += [&]
	{
		loadMenuList.m_widgets.clear();

		for (const auto& entry : std::filesystem::directory_iterator(m_layoutsPath))
		{
			if (entry.is_regular_file() && entry.path().extension() == ".ini")
			{
				std::shared_ptr<std::string> layoutFileName = std::make_shared<std::string>(entry.path().filename().string());

				auto& layoutMenuItem = loadMenuList.CreateWidget<AmberUI::Widgets::MenuItem>(*layoutFileName);
				layoutMenuItem.Name = AmberTools::Utils::String::RemoveExtensionFromFileName(*layoutFileName);

				layoutMenuItem.ClickedEvent += [this, layoutFileName]
				{
					EDITOR_EXEC(DelayAction(std::bind(&AmberUI::Core::UIManager::SetLayout, manager, m_layoutsPath + *layoutFileName), 1));
				};

				auto& contextualMenu = layoutMenuItem.CreateContextualMenu<AmberUI::Widgets::ContextualMenuItem>();
				auto& deleteMenuItem = contextualMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Delete");

				deleteMenuItem.ClickedEvent += [this, layoutFileName, &layoutMenuItem]
				{
					//EDITOR_EXEC(ResetToDefaultLayout);
					EDITOR_EXEC(DelayAction(std::bind(&AmberUI::Core::UIManager::DeleteLayout, manager, m_layoutsPath + *layoutFileName), 1));
					layoutMenuItem.Enabled = false;
					
				};
				auto& renameToMenuList = contextualMenu.CreateWidget<AmberUI::Widgets::MenuList>("Rename to...");

				auto& renameInputText = renameToMenuList.CreateWidget<AmberUI::Widgets::InputText>("");
				renameInputText.Content = AmberTools::Utils::String::RemoveExtensionFromFileName(*layoutFileName);
				renameInputText.SelectAllOnClick = true;

				renameInputText.EnterPressedEvent += [this, layoutFileName, &contextualMenu, &layoutMenuItem](std::string p_newName)
				{
					if (p_newName.empty())
						return;

					layoutMenuItem.Name = p_newName;
					//EDITOR_EXEC(ResetToDefaultLayout);
					std::string oldFileName = m_layoutsPath + *layoutFileName;
					std::string newFileName = m_layoutsPath + p_newName + ".ini";
					EDITOR_EXEC(DelayAction(std::bind(&AmberUI::Core::UIManager::RenameLayout, manager, oldFileName, newFileName), 1));

					*layoutFileName = p_newName + ".ini";
					contextualMenu.Close();
				};
			}
		}
	};

	layoutMenuList.CreateWidget<AmberUI::Widgets::MenuItem>("Reset").ClickedEvent += EDITOR_BIND(ResetToDefaultLayout);
}

void AmberEditor::Panels::MenuBar::CreateResourcesMenu()
{
	auto& resourcesMenu = CreateWidget<AmberUI::Widgets::MenuList>("Resources");
	resourcesMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Compile shaders").ClickedEvent += EDITOR_BIND(CompileShaders);
}

void AmberEditor::Panels::MenuBar::UpdateToggleableItems()
{
	for (auto&[name, panel] : m_panels)
		panel.second.get().Checked = panel.first.get().IsOpened();
}

void AmberEditor::Panels::MenuBar::OpenEveryWindows(bool p_state)
{
	for (auto&[name, panel] : m_panels)
		panel.first.get().SetOpened(p_state);
}

void AmberEditor::Panels::MenuBar::RegisterPanel(const std::string& p_name, AmberUI::Panels::APanelWindow& p_panel)
{
	auto& menuItem = m_windowMenu->CreateWidget<AmberUI::Widgets::MenuItem>(p_name, "", true, true);
	menuItem.ValueChangedEvent += std::bind(&AmberUI::Panels::APanelWindow::SetOpened, &p_panel, std::placeholders::_1);
	
	m_panels.emplace(p_name, std::make_pair(std::ref(p_panel), std::ref(menuItem)));
}