#pragma once

#include "AmberEngine/UI/WidgetContainer.h"

namespace AmberEngine::UI::Widgets
{
	class AContextualMenu : public WidgetContainer
	{
	public:
		virtual ~AContextualMenu() override = default;

		virtual void Close();

		virtual void Draw() = 0;
	};
}