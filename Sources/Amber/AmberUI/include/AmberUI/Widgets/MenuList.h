#pragma once

#include "AmberUI/Widgets/Group.h"

#include "AmberTools/Eventing/Event.h"

namespace AmberUI::Widgets
{
	class MenuList : public Group
	{
	public:
		MenuList(const std::string& p_name, bool p_locked = false);
		virtual ~MenuList() override = default;
	protected:
		virtual void DrawImplementation() override;

	public:
		std::string Name;
		bool Locked;
		AmberTools::Eventing::Event<> ClickedEvent;

	private:
		bool m_opened;
	};

}