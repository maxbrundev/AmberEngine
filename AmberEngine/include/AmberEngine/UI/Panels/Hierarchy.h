#pragma once

#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/UI/Widgets/TreeNode.h"

namespace AmberEngine::UI::Panels
{
	class Hierarchy : public APanelWindow
	{
	public:
		Hierarchy(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings);
		virtual ~Hierarchy() override = default;

		void Clear();
		void AttachActorNodeToParentNode(AmberEngine::Core::ECS::Actor& p_actor);
		void DetachActorNodeFromParentNode(AmberEngine::Core::ECS::Actor& p_actor);
		void AddActorByInstance(AmberEngine::Core::ECS::Actor& p_actor);
		void DeleteActorByInstance(AmberEngine::Core::ECS::Actor& p_actor);

		void SelectActor(AmberEngine::Core::ECS::Actor& p_target);

	public:
		Eventing::Event<AmberEngine::Core::ECS::Actor&> SelectActorEvent;

	private:
		uint64_t m_destroyedListener;

		Widgets::TreeNode* m_root;

		std::unordered_map<AmberEngine::Core::ECS::Actor*, Widgets::TreeNode*> m_widgetActorLink;
	};
}
