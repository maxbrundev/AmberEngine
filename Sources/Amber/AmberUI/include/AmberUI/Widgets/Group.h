#pragma once

#include "AmberUI/WidgetContainer.h"

namespace AmberUI::Widgets
{
	class Group : public AWidget, public WidgetContainer
	{
	public:
		Group() = default;
		virtual ~Group() override = default;
	protected:
		virtual void DrawImplementation() override;
	};
}
