#pragma once

#include "AmberUI/Panels/APanelWindow.h"
#include "AmberUI/Widgets/TreeNode.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberUI/Widgets/Text.h"

namespace AmberEditor::Panels
{
	class Hierarchy : public AmberUI::Panels::APanelWindow
	{
	public:
		Hierarchy(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings);
		virtual ~Hierarchy() override;

		void Clear();
		void AttachActorNodeToParentNode(AmberCore::ECS::Actor& p_actor, AmberCore::ECS::Actor& p_parentActor);
		void DetachActorNodeFromParentNode(AmberCore::ECS::Actor& p_actor, AmberCore::ECS::Actor* p_parentActor);
		void AddActorByInstance(AmberCore::ECS::Actor& p_actor);
		void DeleteActorByInstance(AmberCore::ECS::Actor& p_actor);
		void SelectActor(AmberCore::ECS::Actor& p_target);
		void OnSceneNameChanged(const std::string& p_sceneName);
	public:
		AmberTools::Eventing::Event<AmberCore::ECS::Actor&> SelectActorEvent;

	private:
		void SetupActorTreeNode(AmberUI::Widgets::TreeNode& p_node, AmberCore::ECS::Actor* p_actor);
		void ReorderActorNode(AmberCore::ECS::Actor& p_draggedActor, AmberUI::Widgets::TreeNode& p_draggedNode, AmberCore::ECS::Actor& p_targetActor, AmberUI::Widgets::TreeNode& p_targetNode, bool p_insertBefore);

	private:
		uint64_t m_destroyedListener;
		uint64_t m_sceneNameChangedEventListerId;

		AmberUI::Widgets::TreeNode* m_root;
		AmberUI::Widgets::Text* m_sceneNameText;
		std::unordered_map<AmberCore::ECS::Actor*, AmberUI::Widgets::TreeNode*> m_widgetActorLink;
	};
}
