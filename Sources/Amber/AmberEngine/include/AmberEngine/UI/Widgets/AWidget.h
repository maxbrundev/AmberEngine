#pragma once

namespace AmberEngine::UI
{
	class WidgetContainer;
}

namespace AmberEngine::UI::Widgets
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

	protected:
		virtual void DrawImplementation() = 0;

	public:
		bool enabled = true;
		bool lineBreak = true;

	protected:
		WidgetContainer* m_parent;
		std::string m_widgetID = "?";
		bool m_autoExecutePlugins = true;

	private:
		static uint64_t WIDGET_ID_INCREMENT;
		bool m_destroyed = false;
	};
}