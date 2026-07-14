#pragma once

#ifdef _DEBUG

#include "AmberCore/Rendering/ECSRenderer.h"
#include "AmberWindowing/Context/Window.h"

#include "AmberUI/Panels/PanelUndecorated.h"
#include "AmberUI/Widgets/TextColored.h"

namespace AmberGame::Debug
{
	class FrameInfo : public AmberUI::Panels::PanelUndecorated
	{
	public:
		FrameInfo(AmberCore::Rendering::ECSRenderer& p_renderer, AmberWindowing::Context::Window& p_window);

		void Update(float p_deltaTime);

	private:
		AmberCore::Rendering::ECSRenderer& m_renderer;
		AmberWindowing::Context::Window& m_window;

		AmberUI::Widgets::TextColored* m_frameInfo[3];
	};
}

#endif
