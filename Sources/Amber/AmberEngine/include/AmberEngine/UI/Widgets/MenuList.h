#pragma once

#include "AmberEngine/UI/Widgets/Group.h"

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::UI::Widgets
{
	class MenuList : public Group
	{
	public:
		MenuList(const std::string& p_name, bool p_locked = false);
		virtual ~MenuList() override = default;
	protected:
		virtual void DrawImplementation() override;

	public:
		std::string name;
		bool locked;
		Eventing::Event<> ClickedEvent;

	private:
		bool m_opened;
	};

}