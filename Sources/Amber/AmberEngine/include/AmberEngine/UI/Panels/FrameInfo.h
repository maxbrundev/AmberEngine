#pragma once

#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/UI/Panels/PanelSettings.h"
#include "AmberEngine/UI/Widgets/TextColored.h"

namespace AmberEngine::UI::Panels
{
	class FrameInfo : public APanelWindow
	{
	public:
		FrameInfo(const std::string& p_title, bool p_opened, const PanelSettings& p_windowSettings);

		void Update(float p_deltaTime) const;

	private:
		Widgets::TextColored* m_batchCountText;
		Widgets::TextColored* m_instanceCountText;
		Widgets::TextColored* m_polyCountText;
		Widgets::TextColored* m_vertexCountText;
	};
}
