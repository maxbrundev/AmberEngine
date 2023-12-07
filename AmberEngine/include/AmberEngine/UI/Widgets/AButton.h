#pragma once

#include "AmberEngine/UI/Widgets/AWidget.h"

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::UI::Widgets::Buttons
{
	class AButton : public AWidget
	{
	public:
		AButton() = default;
		virtual ~AButton() override = default;

	protected:
		virtual void DrawImplementation() override = 0;

	public:
		Eventing::Event<> ClickedEvent;
	};
}
