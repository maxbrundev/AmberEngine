#pragma once

#include "AmberEngine/UI/Widgets/AContextualMenu.h"
#include "AmberEngine/UI/Widgets/AWidget.h"

namespace AmberEngine::UI::Widgets
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
