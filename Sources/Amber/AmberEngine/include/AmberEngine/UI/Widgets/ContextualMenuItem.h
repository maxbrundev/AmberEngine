#pragma once

#include "AmberEngine/UI/Widgets/AContextualMenu.h"

namespace AmberEngine::UI::Widgets
{
	class ContextualMenuItem : public AContextualMenu
	{
	public:
		ContextualMenuItem() = default;
		virtual ~ContextualMenuItem() override = default;

		void Draw() override;
	};
}
