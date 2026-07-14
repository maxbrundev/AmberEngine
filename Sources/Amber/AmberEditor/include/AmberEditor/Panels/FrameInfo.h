#pragma once

#include "AmberUI/Panels/APanelWindow.h"
#include "AmberUI/Panels/PanelSettings.h"
#include "AmberUI/Widgets/TextColored.h"

namespace AmberEditor::Panels
{
	class FrameInfo : public AmberUI::Panels::APanelWindow
	{
	public:
		FrameInfo(const std::string& p_title, bool p_opened, const AmberUI::Panels::PanelSettings& p_windowSettings);

		void Update(float p_deltaTime) const;

	private:
		AmberUI::Widgets::TextColored* m_batchCountText;
		AmberUI::Widgets::TextColored* m_instanceCountText;
		AmberUI::Widgets::TextColored* m_polyCountText;
		AmberUI::Widgets::TextColored* m_vertexCountText;
	};
}
