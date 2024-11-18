#pragma once
#include "APanelWindow.h"
#include "AmberEngine/UI/Widgets/ButtonImage.h"

namespace AmberEngine::UI::Panels
{
	class Toolbar : public APanelWindow
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
			const PanelSettings& p_windowSettings
		);

		/**
		* Custom implementation of the draw method
		*/
		void DrawImplementation() override;

	private:
		Widgets::ButtonImage* m_playButton;
		Widgets::ButtonImage* m_pauseButton;
		Widgets::ButtonImage* m_stopButton;
		Widgets::ButtonImage* m_nextButton;
	};
}
