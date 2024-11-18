#pragma once

#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/UI/Widgets/TreeNode.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/UI/Widgets/Text.h"

namespace AmberEngine::UI::Panels
{
	class Hierarchy : public APanelWindow
	{
	public:
		Hierarchy(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings);
		virtual ~Hierarchy() override;

		void Clear();
		void AttachActorNodeToParentNode(AmberEngine::Core::ECS::Actor& p_actor, AmberEngine::Core::ECS::Actor& p_parentActor);
		void DetachActorNodeFromParentNode(AmberEngine::Core::ECS::Actor& p_actor, AmberEngine::Core::ECS::Actor* p_parentActor);
		void AddActorByInstance(AmberEngine::Core::ECS::Actor& p_actor);
		void DeleteActorByInstance(AmberEngine::Core::ECS::Actor& p_actor);
		void SelectActor(AmberEngine::Core::ECS::Actor& p_target);
		void OnSceneNameChanged(const std::string& p_sceneName);
	public:
		Eventing::Event<AmberEngine::Core::ECS::Actor&> SelectActorEvent;

	private:
		uint64_t m_destroyedListener;
		uint64_t m_sceneNameChangedEventListerId;

		Widgets::TreeNode* m_root;
		Widgets::Text* m_sceneNameText;
		std::unordered_map<AmberEngine::Core::ECS::Actor*, Widgets::TreeNode*> m_widgetActorLink;
	};
}
