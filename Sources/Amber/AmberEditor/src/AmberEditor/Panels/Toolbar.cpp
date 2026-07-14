#include "Amberpch.h"

#include "AmberEditor/Panels/Toolbar.h"

#include "AmberEditor/Core/EditorAction.h"
#include "AmberUI/Widgets/Spacing.h"

AmberEditor::Panels::Toolbar::Toolbar(const std::string& p_title, bool p_opened,
                                          const AmberUI::Panels::PanelSettings& p_windowSettings) : AmberUI::Panels::APanelWindow(p_title, p_opened, p_windowSettings)
{
	std::string iconFolder = ":Textures/Icons/";

	auto& textureManager = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::TextureManager>();

	m_playButton = &CreateWidget<AmberUI::Widgets::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Play")->ID, glm::vec2 { 20, 20 });
	m_pauseButton = &CreateWidget<AmberUI::Widgets::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Pause")->ID, glm::vec2 { 20, 20 });
	m_stopButton = &CreateWidget<AmberUI::Widgets::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Stop")->ID, glm::vec2 { 20, 20 });
	m_nextButton = &CreateWidget<AmberUI::Widgets::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Next")->ID, glm::vec2 { 20, 20 });

	CreateWidget<AmberUI::Widgets::Spacing>(0).LineBreak = false;
	auto& refreshButton = CreateWidget<AmberUI::Widgets::ButtonImage>(EDITOR_CONTEXT(editorResources)->GetTexture("Button_Refresh")->ID, glm::vec2 { 20, 20 });

	m_playButton->LineBreak = false;
	m_pauseButton->LineBreak = false;
	m_stopButton->LineBreak = false;
	m_nextButton->LineBreak = false;
	refreshButton.LineBreak = false;

	m_playButton->ClickedEvent += EDITOR_BIND(StartPlaying);
	m_pauseButton->ClickedEvent += EDITOR_BIND(PauseGame);
	m_stopButton->ClickedEvent += EDITOR_BIND(StopPlaying);
	m_nextButton->ClickedEvent += EDITOR_BIND(NextFrame);
	refreshButton.ClickedEvent += EDITOR_BIND(RefreshScripts);

	EDITOR_EVENT(EditorModeChangedEvent) += [this](AmberEditor::Core::EditorAction::EEditorMode p_newMode)
	{
		auto enable = [](AmberUI::Widgets::ButtonImage* p_button, bool p_enable)
		{
			p_button->Disabled = !p_enable;
			p_button->Tint = p_enable ? AmberRendering::Data::Color{ 1.0f, 1.0f, 1.0f, 1.0f } : AmberRendering::Data::Color{ 1.0f, 1.0f, 1.0f, 0.15f };
		};

		switch (p_newMode)
		{
		case AmberEditor::Core::EditorAction::EEditorMode::EDIT:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, false);
			enable(m_nextButton, false);
			break;
		case AmberEditor::Core::EditorAction::EEditorMode::PLAY:
			enable(m_playButton, false);
			enable(m_pauseButton, true);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		case AmberEditor::Core::EditorAction::EEditorMode::PAUSE:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		case AmberEditor::Core::EditorAction::EEditorMode::FRAME_BY_FRAME:
			enable(m_playButton, true);
			enable(m_pauseButton, false);
			enable(m_stopButton, true);
			enable(m_nextButton, true);
			break;
		}
	};

	EDITOR_EXEC(SetEditorMode(AmberEditor::Core::EditorAction::EEditorMode::EDIT));
}

void AmberEditor::Panels::Toolbar::DrawImplementation()
{
	AmberUI::Panels::APanelWindow::DrawImplementation();
}
