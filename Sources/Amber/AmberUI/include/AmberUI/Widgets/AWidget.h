#pragma once

#include "AmberUI/DragDrop.h"

namespace AmberUI
{
	class WidgetContainer;
}

namespace AmberUI::Widgets
{
	class AWidget
	{
	public:
		AWidget();
		virtual ~AWidget() = default;

		virtual void Draw();
		void LinkTo(const AWidget& p_widget);
		void Destroy();
		bool IsDestroyed() const;
		void SetParent(WidgetContainer* p_parent);
		bool HasParent() const;
		WidgetContainer* GetParent() const;

		DragDropSource& SetDragDropSource(const std::string& p_identifier, const std::string& p_tooltip, const std::pair<std::string, Group*>& p_data);
		DragDropTarget& AddDragDropTarget(const std::string& p_identifier);
		void ClearDragDrop();

	protected:
		virtual void DrawImplementation() = 0;

		void DrawDragDrop();

	public:
		bool Enabled = true;
		bool LineBreak = true;

		AmberTools::Eventing::Event<> DrawnEvent;

	protected:
		WidgetContainer* m_parent;
		std::string m_widgetID = "?";
		bool m_autoExecuteDragDrop = true;

	private:
		static uint64_t WIDGET_ID_INCREMENT;
		bool m_destroyed = false;
		std::unique_ptr<DragDropSource> m_dragDropSource;
		std::vector<std::unique_ptr<DragDropTarget>> m_dragDropTargets;
	};
}
