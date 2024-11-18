#pragma once

#include "AmberEngine/UI/ContextualMenuContainer.h"
#include "AmberEngine/UI/Widgets/AWidget.h"
#include "AmberEngine/UI/WidgetContainer.h"
#include "AmberEngine/UI/Core/IPlugin.h"


namespace AmberEngine::Core::ECS
{
	class Actor;
}

namespace AmberEngine::UI::Widgets
{
	class TreeNode : public AWidget, public WidgetContainer, public ContextualMenuContainer
	{
	public:
		TreeNode(const std::string& p_name, bool p_arrowClickToOpen = false);
		virtual ~TreeNode() override = default;

		void SetActor(AmberEngine::Core::ECS::Actor* p_actor);

		void Update();

		void Open();

		void Close();

		bool IsOpened() const;

	protected:
		virtual void DrawImplementation() override;

	public:
		std::string name;
		std::string& m_actorName;

		bool isSelected = false;
		bool isLeaf     = false;

		Eventing::Event<> ClickedEvent;
		Eventing::Event<> DoubleClickedEvent;
		Eventing::Event<> OpenedEvent;
		Eventing::Event<> ClosedEvent;
		Eventing::Event<TreeNode*> UpEvent;
		Eventing::Event<TreeNode*> DownEvent;

		AmberEngine::Core::ECS::Actor* m_actor;
		bool isReorder = false;
	private:
		bool m_isArrowClickToOpen = false;
		bool m_isShouldOpen       = false;
		bool m_isShouldClose      = false;
		bool m_isOpened           = false;

		std::pair<AmberEngine::Core::ECS::Actor*, TreeNode*> m_data;

		ImGuiDragDropFlags target_flags = 0;
	};
}