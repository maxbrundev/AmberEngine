#pragma once
#include "AmberUI/Panels/APanelWindow.h"
#include "AmberUI/Widgets/ButtonImage.h"

namespace AmberEditor::Panels
{
	class Toolbar : public AmberUI::Panels::APanelWindow
	{
	public:
		/**
		* Constructor
		* @param p_title
		* @param p_opened
		* @param p_windowSettings
		*/
		Toolbar
		(
			const std::string& p_title,
			bool p_opened,
			const AmberUI::Panels::PanelSettings& p_windowSettings
		);

		/**
		* Custom implementation of the draw method
		*/
		void DrawImplementation() override;

	private:
		AmberUI::Widgets::ButtonImage* m_playButton;
		AmberUI::Widgets::ButtonImage* m_pauseButton;
		AmberUI::Widgets::ButtonImage* m_stopButton;
		AmberUI::Widgets::ButtonImage* m_nextButton;
	};
}
