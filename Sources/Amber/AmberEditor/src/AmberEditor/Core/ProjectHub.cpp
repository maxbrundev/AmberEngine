#include "Amberpch.h"

#include "AmberEditor/Core/ProjectHub.h"

#include "AmberTools/Utils/PathParser.h"
#include "AmberUI/Widgets/Button.h"
#include "AmberUI/Widgets/Column.h"
#include "AmberUI/Widgets/Group.h"
#include "AmberUI/Widgets/InputText.h"
#include "AmberUI/Widgets/Separator.h"
#include "AmberUI/Widgets/Spacing.h"
#include "AmberUI/Widgets/Text.h"
#include "AmberWindowing/Dialogs/OpenFileDialog.h"
#include "AmberWindowing/Dialogs/SaveFileDialog.h"
#include "AmberWindowing/Dialogs/MessageBox.h"

#define PROJECTS_FILE std::string(std::string(getenv("APPDATA")) + "\\AmberTech\\ABEditor\\projects.ini")

class ProjectHubPanel : public AmberUI::Panels::APanelWindow
{
public:
	ProjectHubPanel(bool& p_readyToGo, std::string& p_path, std::string& p_projectName) :
	AmberUI::Panels::APanelWindow("Amber - Project Hub", true),
	m_readyToGo(p_readyToGo),
	m_path(p_path),
	m_projectName(p_projectName)
	{
		Resizable = false;
		Movable = false;
		TitleBar = false;

		std::filesystem::create_directories(std::string(getenv("APPDATA")) + "\\AmberTech\\ABEditor\\");

		SetSize({ 1000, 580 });
		SetPosition({ 0.f, 0.f });

		auto& openProjectButton = CreateWidget<AmberUI::Widgets::Button>("Open Project");
		auto& newProjectButton = CreateWidget<AmberUI::Widgets::Button>("New Project");
		auto& pathField = CreateWidget<AmberUI::Widgets::InputText>("");
		m_goButton = &CreateWidget<AmberUI::Widgets::Button>("GO");

		pathField.ContentChangedEvent += [this, &pathField](std::string p_content)
		{
			pathField.Content = AmberTools::Utils::PathParser::MakeWindowsStyle(p_content);

			if (pathField.Content != "" && pathField.Content.back() != '\\')
				pathField.Content += '\\';

			UpdateGoButton(pathField.Content);
		};

		UpdateGoButton("");

		openProjectButton.IdleBackgroundColor = { 0.7f, 0.5f, 0.f };
		newProjectButton.IdleBackgroundColor = { 0.f, 0.5f, 0.0f };

		openProjectButton.ClickedEvent += [this]
		{
			AmberWindowing::Dialogs::OpenFileDialog dialog("Open project");
			dialog.AddFileType("Amber Project", "*.abproject");
			dialog.Show();

			std::string abProjectPath = dialog.GetSelectedFilePath();
			std::string rootFolderPath = AmberTools::Utils::PathParser::GetContainingFolder(abProjectPath);

			if (dialog.HasSucceeded())
			{
				RegisterProject(rootFolderPath);
				OpenProject(rootFolderPath);
			}
		};

		newProjectButton.ClickedEvent += [this, &pathField]
		{
			AmberWindowing::Dialogs::SaveFileDialog dialog("New project location");
			dialog.DefineExtension("Amber Project", "..");
			dialog.Show();
			if (dialog.HasSucceeded())
			{
				std::string result = dialog.GetSelectedFilePath();
				pathField.Content = std::string(result.data(), result.data() + result.size() - std::string("..").size()); // remove auto extension
				pathField.Content += "\\";
				UpdateGoButton(pathField.Content);
			}
		};

		m_goButton->ClickedEvent += [this, &pathField]
		{
			CreateProject(pathField.Content);
			RegisterProject(pathField.Content);
			OpenProject(pathField.Content);
		};

		openProjectButton.LineBreak = false;
		newProjectButton.LineBreak = false;
		pathField.LineBreak = false;

		for (uint8_t i = 0; i < 4; ++i)
			CreateWidget<AmberUI::Widgets::Spacing>();

		CreateWidget<AmberUI::Widgets::Separator>();

		for (uint8_t i = 0; i < 4; ++i)
			CreateWidget<AmberUI::Widgets::Spacing>();

		auto& columns = CreateWidget<AmberUI::Widgets::Columns<2>>();

		columns.Widths = { 750, 500 };

		std::string line;
		std::ifstream myfile(PROJECTS_FILE);
		if (myfile.is_open())
		{
			while (getline(myfile, line))
			{
				if (std::filesystem::exists(line)) // TODO: Delete line from the file
				{
					auto& text = columns.CreateWidget<AmberUI::Widgets::Text>(line);
					auto& actions = columns.CreateWidget<AmberUI::Widgets::Group>();
					auto& openButton = actions.CreateWidget<AmberUI::Widgets::Button>("Open");
					auto& deleteButton = actions.CreateWidget<AmberUI::Widgets::Button>("Delete");

					openButton.IdleBackgroundColor = { 0.7f, 0.5f, 0.f };
					deleteButton.IdleBackgroundColor = { 0.5f, 0.f, 0.f };

					openButton.ClickedEvent += [this, line]
					{
						OpenProject(line);
					};

					std::string toErase = line;
					deleteButton.ClickedEvent += [this, &text, &actions, toErase]
					{
						text.Destroy();
						actions.Destroy();

						std::string line;
						std::ifstream fin(PROJECTS_FILE);
						std::ofstream temp("temp");

						while (getline(fin, line))
							if (line != toErase)
								temp << line << std::endl;

						temp.close();
						fin.close();

						std::filesystem::remove(PROJECTS_FILE);
						std::filesystem::rename("temp", PROJECTS_FILE);
					};

					openButton.LineBreak = false;
					deleteButton.LineBreak;
				}
			}
			myfile.close();
		}
	}

	void UpdateGoButton(const std::string& p_path)
	{
		bool validPath = p_path != "";
		m_goButton->IdleBackgroundColor = validPath ? AmberRendering::Data::Color{ 0.f, 0.5f, 0.0f } : AmberRendering::Data::Color{ 0.1f, 0.1f, 0.1f };
		m_goButton->Disabled = !validPath;
	}

	void CreateProject(const std::string& p_path)
	{
		if (!std::filesystem::exists(p_path))
		{
			std::filesystem::create_directory(p_path);
			std::filesystem::create_directory(p_path + "Assets\\");
			std::filesystem::create_directory(p_path + "Scripts\\");
			std::ofstream projectFile(p_path + '\\' + AmberTools::Utils::PathParser::GetElementName(std::string(p_path.data(), p_path.data() + p_path.size() - 1)) + ".abproject");
		}
	}

	void RegisterProject(const std::string& p_path)
	{
		bool pathAlreadyRegistered = false;

		{
			std::string line;
			std::ifstream myfile(PROJECTS_FILE);
			if (myfile.is_open())
			{
				while (getline(myfile, line))
				{
					if (line == p_path)
					{
						pathAlreadyRegistered = true;
						break;
					}
				}
				myfile.close();
			}
		}

		if (!pathAlreadyRegistered)
		{
			std::ofstream projectsFile(PROJECTS_FILE, std::ios::app);
			projectsFile << p_path << std::endl;
		}
	}

	void OpenProject(const std::string& p_path)
	{
		m_readyToGo = std::filesystem::exists(p_path);
		if (!m_readyToGo)
		{
			
			AmberWindowing::Dialogs::MessageBox errorMessage("Project not found", "The selected project does not exists", AmberWindowing::Dialogs::MessageBox::EMessageType::ERROR, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK);
		}
		else
		{
			m_path = p_path;
			m_projectName = AmberTools::Utils::PathParser::GetElementName(m_path);
			Close();
		}
	}

	void Draw() override
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 50.f, 50.f });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

		AmberUI::Panels::APanelWindow::Draw();

		ImGui::PopStyleVar(2);
	}

private:
	bool& m_readyToGo;
	std::string& m_path;
	std::string& m_projectName;
	AmberUI::Widgets::Button* m_goButton = nullptr;
};

AmberEditor::Core::ProjectHub::ProjectHub()
{
	SetupContext();
	m_mainPanel = std::make_unique<ProjectHubPanel>(m_readyToGo, m_projectPath, m_projectName);

	m_uiManager->SetCanvas(m_canvas);
	m_canvas.AddPanel(*m_mainPanel);
}

std::tuple<bool, std::string, std::string> AmberEditor::Core::ProjectHub::Run()
{
	m_renderer->SetClearColor(0.f, 0.f, 0.f, 1.f);

	while (!m_window->ShouldClose())
	{
		m_renderer->Clear();
		m_device->PollEvents();
		m_uiManager->Render();
		m_uiManager->PostRender();
		m_window->SwapBuffers();

		if (!m_mainPanel->IsOpened())
			m_window->SetShouldClose(true);
	}

	return { m_readyToGo, m_projectPath, m_projectName };
}

void AmberEditor::Core::ProjectHub::SetupContext()
{
	/* Settings */
	AmberWindowing::Settings::DeviceSettings deviceSettings;
	AmberWindowing::Settings::WindowSettings windowSettings;
	windowSettings.title = "Amber - Project Hub";
	windowSettings.width = 1000;
	windowSettings.height = 580;
	windowSettings.fullScreen = false;
	windowSettings.resizable = false;
	windowSettings.decorated = true;

	/* Window creation */
	m_device = std::make_unique<AmberWindowing::Context::Device>(deviceSettings);
	m_window = std::make_unique<AmberWindowing::Context::Window>(*m_device, windowSettings);
	m_window->MakeCurrentContext();

	auto[monWidth, monHeight] = m_device->GetMonitorSize();
	auto[winWidth, winHeight] = m_window->GetSize();
	m_window->SetPosition(monWidth / 2 - winWidth / 2, monHeight / 2 - winHeight / 2);

	/* Graphics context creation */
	m_driver = std::make_unique<AmberRendering::Context::Driver>(AmberRendering::Settings::DriverSettings{ false });
	m_renderer = std::make_unique<AmberRendering::Core::Renderer>(*m_driver);
	m_driver->SetCapability(AmberRendering::Settings::ERenderingCapability::MULTISAMPLE, true);

	m_uiManager = std::make_unique<AmberUI::Core::UIManager>(m_window->GetGlfwWindow());
	m_uiManager->LoadFont("Ruda_Big", "Data\\Editor\\Fonts\\Ruda-Bold.ttf", 18);
	m_uiManager->UseFont("Ruda_Big");
	m_uiManager->EnableEditorLayoutSave(false);
	m_uiManager->EnableDocking(false);
}

void AmberEditor::Core::ProjectHub::RegisterProject(const std::string& p_path)
{
	static_cast<ProjectHubPanel*>(m_mainPanel.get())->RegisterProject(p_path);
}
