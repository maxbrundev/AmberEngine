#pragma once

#include "AmberTools/Eventing/Event.h"

namespace AmberUI::Widgets
{
	class Group;
}

namespace AmberUI
{
	class DragDropSource
	{
	public:
		DragDropSource(const std::string& p_identifier, const std::string& p_tooltip, const std::pair<std::string, Widgets::Group*>& p_data);

		void Draw();

		bool IsDragged() const;

	public:
		std::string Identifier;
		std::string Tooltip;
		std::pair<std::string, Widgets::Group*> Data;

		AmberTools::Eventing::Event<> DragStartEvent;
		AmberTools::Eventing::Event<> DragStopEvent;

		bool HasTooltip = true;

	private:
		bool m_isDragged = false;
	};

	class DragDropTarget
	{
	public:
		DragDropTarget(const std::string& p_identifier);

		void Draw();

		bool IsHovered() const;

	public:
		std::string Identifier;

		AmberTools::Eventing::Event<std::pair<std::string, Widgets::Group*>> DataReceivedEvent;
		AmberTools::Eventing::Event<> HoverStartEvent;
		AmberTools::Eventing::Event<> HoverEndEvent;

		bool ShowYellowRect = true;

	private:
		bool m_isHovered = false;
	};
}
