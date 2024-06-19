#pragma once

#include "AmberEngine/UI/WidgetContainer.h"

namespace AmberEngine::UI::Widgets
{
	class ContextualMenuWindow : public AWidget, public WidgetContainer
	{
	public:
		ContextualMenuWindow() = default;
		virtual ~ContextualMenuWindow() override = default;

		void Close();

	protected:
		void DrawImplementation() override;
	};
}
