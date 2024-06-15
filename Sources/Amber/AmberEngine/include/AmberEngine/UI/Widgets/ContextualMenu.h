#pragma once

#include "AmberEngine/UI/WidgetContainer.h"

namespace AmberEngine::UI::Widgets
{
	class ContextualMenu : public AWidget, public WidgetContainer
	{
	public:
		ContextualMenu() = default;
		virtual ~ContextualMenu() override = default;

		void Close();

	protected:
		void DrawImplementation() override;
	};
}
