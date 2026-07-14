#pragma once

#include "AmberUI/Widgets/AContextualMenu.h"

namespace AmberUI::Widgets
{
	class ContextualMenuItem : public AContextualMenu
	{
	public:
		ContextualMenuItem() = default;
		virtual ~ContextualMenuItem() override = default;

		void Draw() override;
	};
}
