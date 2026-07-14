#pragma once

#include "AmberUI/Panels/PanelUndecorated.h"
#include "AmberUI/Widgets/TextColored.h"

#include "AmberWindowing/Context/Window.h"

namespace AmberGame::Utils
{
	class FPSCounter : public AmberUI::Panels::PanelUndecorated
	{
	public:
		FPSCounter(AmberWindowing::Context::Window& p_window);

		void Update(float p_deltaTime);

	private:
		AmberUI::Widgets::TextColored* m_text;

		AmberWindowing::Context::Window& m_window;
		float m_elapsed = 0.0f;
		uint32_t m_frames = 0;
	};
}
