#pragma once

#include "AmberEngine/UI/Panels/APanel.h"

namespace AmberEngine::UI::Panels
{
	class PanelMenuBar : public APanel
	{
	public:
		PanelMenuBar() = default;
		virtual ~PanelMenuBar() override = default;

	protected:
		void DrawImplementation() override;
	};
}
