#pragma once

#include "AmberUI/WidgetContainer.h"

namespace AmberUI::Widgets
{
	class AContextualMenu : public WidgetContainer
	{
	public:
		virtual ~AContextualMenu() override = default;

		virtual void Close();

		virtual void Draw() = 0;
	};
}