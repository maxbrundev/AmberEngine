#include "Amberpch.h"

#include "AmberEditor/Panels/ProjectSettings.h"

#include "AmberEditor/Core/EditorAction.h"
#include "AmberCore/Helpers/GUIDrawer.h"
#include "AmberUI/Widgets/Column.h"
#include "AmberUI/wIDGETS/GroupCollapsable.h"
#include "AmberUI/Widgets/Separator.h"

AmberEditor::Panels::ProjectSettings::ProjectSettings(const std::string& p_title, bool p_opened, const AmberUI::Panels::PanelSettings& p_windowSettings) : AmberUI::Panels::APanelWindow(p_title, p_opened, p_windowSettings), m_projectFile(EDITOR_CONTEXT(projectSettings))
{
	auto& saveButton = CreateWidget<AmberUI::Widgets::Button>("Apply");
	saveButton.IdleBackgroundColor = { 0.0f, 0.5f, 0.0f };
	saveButton.ClickedEvent += [this]
	{
		EDITOR_CONTEXT(ApplyProjectSettings());
		m_projectFile.Rewrite();
	};

	saveButton.LineBreak = false;

	auto& resetButton = CreateWidget<AmberUI::Widgets::Button>("Reset");
	resetButton.IdleBackgroundColor = { 0.5f, 0.0f, 0.0f };
	resetButton.ClickedEvent += [this]
	{
		EDITOR_CONTEXT(ResetProjectSettings());
	};

	CreateWidget<AmberUI::Widgets::Separator>();

	{
		/* Physics settings */
		auto& root = CreateWidget<AmberUI::Widgets::GroupCollapsable>("Physics");
		auto& columns = root.CreateWidget<AmberUI::Widgets::Columns<2>>();
		columns.Widths[0] = 125;

		AmberCore::Helpers::GUIDrawer::DrawScalar<float>(columns, "Gravity", GenerateGatherer<float>("gravity"), GenerateProvider<float>("gravity"), 0.1f, AmberCore::Helpers::GUIDrawer::MIN_FLOAT, AmberCore::Helpers::GUIDrawer::MAX_FLOAT);
	}

	{
		/* Build settings */
		auto& generationRoot = CreateWidget<AmberUI::Widgets::GroupCollapsable>("Build");
		auto& columns = generationRoot.CreateWidget<AmberUI::Widgets::Columns<2>>();
		columns.Widths[0] = 125;

		AmberCore::Helpers::GUIDrawer::DrawBoolean(columns, "Development build", GenerateGatherer<bool>("dev_build"), GenerateProvider<bool>("dev_build"));
	}

	{
		/* Windowing settings */
		auto& windowingRoot = CreateWidget<AmberUI::Widgets::GroupCollapsable>("Windowing");
		auto& columns = windowingRoot.CreateWidget<AmberUI::Widgets::Columns<2>>();
		columns.Widths[0] = 125;

		AmberCore::Helpers::GUIDrawer::DrawScalar<int>(columns, "Resolution X", GenerateGatherer<int>("x_resolution"), GenerateProvider<int>("x_resolution"), 1, 0, 10000);
		AmberCore::Helpers::GUIDrawer::DrawScalar<int>(columns, "Resolution Y", GenerateGatherer<int>("y_resolution"), GenerateProvider<int>("y_resolution"), 1, 0, 10000);
		AmberCore::Helpers::GUIDrawer::DrawBoolean(columns, "Fullscreen", GenerateGatherer<bool>("fullscreen"), GenerateProvider<bool>("fullscreen"));
		AmberCore::Helpers::GUIDrawer::DrawString(columns, "Executable name", GenerateGatherer<std::string>("executable_name"), GenerateProvider<std::string>("executable_name"));
	}

	{
		/* Rendering settings */
		auto& renderingRoot = CreateWidget<AmberUI::Widgets::GroupCollapsable>("Rendering");
		auto& columns = renderingRoot.CreateWidget<AmberUI::Widgets::Columns<2>>();
		columns.Widths[0] = 125;

		AmberCore::Helpers::GUIDrawer::DrawBoolean(columns, "Vertical Sync.", GenerateGatherer<bool>("vsync"), GenerateProvider<bool>("vsync"));
		AmberCore::Helpers::GUIDrawer::DrawBoolean(columns, "Multi-sampling", GenerateGatherer<bool>("multisampling"), GenerateProvider<bool>("multisampling"));
		AmberCore::Helpers::GUIDrawer::DrawScalar<int>(columns, "Samples", GenerateGatherer<int>("samples"), GenerateProvider<int>("samples"), 1, 2, 16);
		AmberCore::Helpers::GUIDrawer::DrawScalar<int>(columns, "OpenGL Major", GenerateGatherer<int>("opengl_major"), GenerateProvider<int>("opengl_major"), 1, 3, 4);
		AmberCore::Helpers::GUIDrawer::DrawScalar<int>(columns, "OpenGL Minor", GenerateGatherer<int>("opengl_minor"), GenerateProvider<int>("opengl_minor"), 1, 0, 6);
	}

	{
		/* Scene Management settings */
		auto& gameRoot = CreateWidget<AmberUI::Widgets::GroupCollapsable>("Scene Management");
		auto& columns = gameRoot.CreateWidget<AmberUI::Widgets::Columns<2>>();
		columns.Widths[0] = 125;

		AmberCore::Helpers::GUIDrawer::DrawDDString(columns, "Start scene", GenerateGatherer<std::string>("start_scene"), GenerateProvider<std::string>("start_scene"), "File");
	}
}
