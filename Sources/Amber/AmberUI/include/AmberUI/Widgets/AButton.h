#pragma once

#include "AmberUI/Widgets/AWidget.h"

#include "AmberTools/Eventing/Event.h"

namespace AmberUI::Widgets
{
	class AButton : public AWidget
	{
	public:
		AButton() = default;
		virtual ~AButton() override = default;

	protected:
		virtual void DrawImplementation() override = 0;

	public:
		AmberTools::Eventing::Event<> ClickedEvent;
	};
}
