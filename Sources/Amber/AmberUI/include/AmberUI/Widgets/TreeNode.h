#pragma once

#include "AmberUI/ContextualMenuContainer.h"
#include "AmberUI/Widgets/AWidget.h"
#include "AmberUI/WidgetContainer.h"

namespace AmberUI::Widgets
{
	class TreeNode : public AWidget, public WidgetContainer, public ContextualMenuContainer
	{
	public:
		TreeNode(const std::string& p_name, bool p_arrowClickToOpen = false);
		virtual ~TreeNode() override = default;

		void Open();

		void Close();

		bool IsOpened() const;

	protected:
		virtual void DrawImplementation() override;

	public:
		std::string Name;

		bool IsSelected = false;
		bool IsLeaf     = false;

		AmberTools::Eventing::Event<> ClickedEvent;
		AmberTools::Eventing::Event<> DoubleClickedEvent;
		AmberTools::Eventing::Event<> OpenedEvent;
		AmberTools::Eventing::Event<> ClosedEvent;

	private:
		bool m_isArrowClickToOpen = false;
		bool m_isShouldOpen       = false;
		bool m_isShouldClose      = false;
		bool m_isOpened           = false;
	};
}
