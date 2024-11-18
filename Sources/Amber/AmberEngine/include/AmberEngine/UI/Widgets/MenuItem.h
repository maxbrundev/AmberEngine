#pragma once

#include "AmberEngine/UI/Widgets/DataWidget.h"
#include "AmberEngine/Eventing/Event.h"
#include "AmberEngine/UI/ContextualMenuContainer.h"
#include "AmberEngine/UI/WidgetContainer.h"

namespace AmberEngine::UI::Widgets
{
	class MenuItem : public DataWidget<bool>, public WidgetContainer, public ContextualMenuContainer
	{
	public:
		MenuItem(const std::string& p_name, const std::string& p_shortcut = "", bool p_checkable = false, bool p_checked = false);
		virtual ~MenuItem() override = default;

	protected:
		void DrawImplementation() override;

	public:
		std::string name;
		std::string shortcut;
		bool checkable;
		bool checked;
		Eventing::Event<> ClickedEvent;
		Eventing::Event<bool> ValueChangedEvent;

	private:
		bool m_selected;
	};
}
