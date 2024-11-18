#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Toolbar.h"

#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/UI/Widgets/Spacing.h"

AmberEngine::UI::Panels::Toolbar::Toolbar(const std::string& p_title, bool p_opened,
                                          const PanelSettings& p_windowSettings) : APanelWindow(p_title, p_opened, p_windowSettings)
{
	std::string iconFolder = ":Textures/Icons/";

	auto& textureManager = Tools::Global::ServiceLocator::Get<ResourceManagement::TextureManager>();

	m_playButton = &CreateWidget<Widgets::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Play")->ID, glm::vec2 { 20, 20 });
	m_pauseButton = &CreateWidget<Widgets::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Pause")->ID, glm::vec2 { 20, 20 });
	m_stopButton = &CreateWidget<Widgets::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Stop")->ID, glm::vec2 { 20, 20 });
	m_nextButton = &CreateWidget<Widgets::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Next")->ID, glm::vec2 { 20, 20 });

	CreateWidget<Widgets::Spacing>(0).lineBreak = false;
	auto& refreshButton = CreateWidget<Widgets::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Refresh")->ID, glm::vec2 { 20, 20 });

	m_playButton->lineBreak = false;
	m_pauseButton->lineBreak = false;
	m_stopButton->lineBreak = false;
	m_nextButton->lineBreak = false;
	refreshButton.lineBreak = false;

	m_playButton->ClickedEvent += EDITOR_BIND(StartPlaying);
	m_pauseButton->ClickedEvent += EDITOR_BIND(PauseGame);
	m_stopButton->ClickedEvent += EDITOR_BIND(StopPlaying);
	m_nextButton->ClickedEvent += EDITOR_BIND(NextFrame);
	//refreshButton.ClickedEvent += EDITOR_BIND(RefreshScripts);

	EDITOR_EVENT(EditorModeChangedEvent) += [this](AmberEngine::Core::EditorAction::EEditorMode p_newMode)
	{
		auto enable = [](Widgets::ButtonImage* p_button, bool p_enable)
		{
			p_button->disabled = !p_enable;
			p_button->tint = p_enable ? Data::Color{ 1.0f, 1.0f, 1.0f, 1.0f } : Data::Color{ 1.0f, 1.0f, 1.0f, 0.15f };
		};

		switch (p_newMode)
		{
		case AmberEngine::Core::EditorAction::EEditorMode::EDIT:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, false);
			enable(m_nextButton, false);
			break;
		case AmberEngine::Core::EditorAction::EEditorMode::PLAY:
			enable(m_playButton, false);
			enable(m_pauseButton, true);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		case AmberEngine::Core::EditorAction::EEditorMode::PAUSE:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		case AmberEngine::Core::EditorAction::EEditorMode::FRAME_BY_FRAME:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		}
	};

	EDITOR_EXEC(SetEditorMode(AmberEngine::Core::EditorAction::EEditorMode::EDIT));
}

void AmberEngine::UI::Panels::Toolbar::DrawImplementation()
{
	APanelWindow::DrawImplementation();
}
