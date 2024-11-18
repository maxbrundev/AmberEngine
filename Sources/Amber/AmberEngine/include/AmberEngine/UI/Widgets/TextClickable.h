#pragma once


#include "Text.h"
#include "AmberEngine/Eventing/Event.h"
#include "AmberEngine/UI/WidgetContainer.h"
#include "AmberEngine/UI/ContextualMenuContainer.h"

namespace AmberEngine::UI::Widgets
{
	class TextClickable : public Text, public WidgetContainer, public ContextualMenuContainer
	{
	public:
		TextClickable(const std::string& p_content = "");
		virtual ~TextClickable() override = default;
		
	protected:
		virtual void DrawImplementation() override;

	public:
		Eventing::Event<> ClickedEvent;
		Eventing::Event<> DoubleClickedEvent;
	};
}
