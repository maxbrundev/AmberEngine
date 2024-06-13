#pragma once

#include "AmberEngine/UI/WidgetContainer.h"

namespace AmberEngine::UI::Widgets
{
	class ContextualMenu : public AWidget, public WidgetContainer
	{
	public:
		ContextualMenu() = default;
		virtual ~ContextualMenu() override = default;

	protected:
		void DrawImplementation() override;
	};
}
