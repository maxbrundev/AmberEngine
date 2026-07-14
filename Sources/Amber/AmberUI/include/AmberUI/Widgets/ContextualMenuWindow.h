#pragma once

#include "AmberUI/Widgets/AContextualMenu.h"
#include "AmberUI/Widgets/AWidget.h"

namespace AmberUI::Widgets
{
	class ContextualMenuWindow : public AWidget, public AContextualMenu
	{
	public:
		ContextualMenuWindow() = default;
		virtual ~ContextualMenuWindow() override = default;

		void Draw() override;

	protected:
		void DrawImplementation() override;
	};
}
