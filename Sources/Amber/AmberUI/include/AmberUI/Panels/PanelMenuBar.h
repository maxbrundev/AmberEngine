#pragma once

#include "AmberUI/Panels/APanel.h"

namespace AmberUI::Panels
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
