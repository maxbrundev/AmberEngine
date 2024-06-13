#pragma once

#include "AmberEngine/UI/WidgetContainer.h"

namespace AmberEngine::UI::Widgets
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
