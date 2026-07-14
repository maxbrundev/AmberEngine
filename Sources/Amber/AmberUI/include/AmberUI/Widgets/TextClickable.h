#pragma once


#include "Text.h"
#include "AmberTools/Eventing/Event.h"
#include "AmberUI/WidgetContainer.h"
#include "AmberUI/ContextualMenuContainer.h"

namespace AmberUI::Widgets
{
	class TextClickable : public Text, public WidgetContainer, public ContextualMenuContainer
	{
	public:
		TextClickable(const std::string& p_content = "");
		virtual ~TextClickable() override = default;
		
	protected:
		virtual void DrawImplementation() override;

	public:
		AmberTools::Eventing::Event<> ClickedEvent;
		AmberTools::Eventing::Event<> DoubleClickedEvent;
	};
}
