#pragma once

#include "AmberUI/Widgets/DataWidget.h"
#include "AmberTools/Eventing/Event.h"
#include "AmberUI/ContextualMenuContainer.h"
#include "AmberUI/WidgetContainer.h"

namespace AmberUI::Widgets
{
	class MenuItem : public DataWidget<bool>, public WidgetContainer, public ContextualMenuContainer
	{
	public:
		MenuItem(const std::string& p_name, const std::string& p_shortcut = "", bool p_checkable = false, bool p_checked = false);
		virtual ~MenuItem() override = default;

	protected:
		void DrawImplementation() override;

	public:
		std::string Name;
		std::string Shortcut;
		bool Checkable;
		bool Checked;
		AmberTools::Eventing::Event<> ClickedEvent;

	private:
	};
}
