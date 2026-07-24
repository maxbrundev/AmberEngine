#pragma once

#include "AmberUI/Widgets/Group.h"

#include "AmberTools/Eventing/Event.h"

namespace AmberUI::Widgets
{
	class GroupCollapsable : public Group
	{
	public:
		GroupCollapsable(const std::string& p_name = "");
		virtual ~GroupCollapsable() override = default;

	protected:
		virtual void DrawImplementation() override;

	public:
		std::string Name;
		bool Closable = false;
		bool Opened = true;
		bool Collapsed = false;
		AmberTools::Eventing::Event<> CloseEvent;
		AmberTools::Eventing::Event<> OpenEvent;
	};
}
