#pragma once

#include "AmberUI/ContextualMenuContainer.h"
#include "AmberUI/WidgetContainer.h"

namespace AmberUI::Panels
{
	class APanel : public WidgetContainer, public ContextualMenuContainer
	{
	public:
		APanel();
		virtual ~APanel() override = default;

		virtual void Draw();

	protected:
		virtual void DrawImplementation() = 0;

	public:
		bool Enabled = true;
	};
}
