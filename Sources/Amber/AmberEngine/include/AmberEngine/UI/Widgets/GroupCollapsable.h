#pragma once

#include "AmberEngine/UI/Widgets/Group.h"

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::UI::Widgets
{
	class GroupCollapsable : public Group
	{
	public:
		GroupCollapsable(const std::string& p_name = "");
		virtual ~GroupCollapsable() override = default;

	protected:
		virtual void DrawImplementation() override;

	public:
		std::string name;
		bool closable = false;
		bool opened = true;
		Eventing::Event<> CloseEvent;
		Eventing::Event<> OpenEvent;
	};
}
