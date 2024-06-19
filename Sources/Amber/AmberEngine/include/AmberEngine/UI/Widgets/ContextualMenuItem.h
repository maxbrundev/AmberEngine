#pragma once

#include "AmberEngine/UI/WidgetContainer.h"

namespace AmberEngine::UI::Widgets
{
	class ContextualMenuItem : public AWidget, public WidgetContainer
	{
	public:
		ContextualMenuItem() = default;
		virtual ~ContextualMenuItem() override = default;

		void Close();

	protected:
		void DrawImplementation() override;
	};
}
