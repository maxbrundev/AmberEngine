#pragma once

#include "AmberEngine/UI/Panels/APanelWindow.h"
#include "AmberEngine/Core/ECS/Actor.h"

class TreeNode;

namespace AmberEngine::UI
{
	class Hierarchy : public APanelWindow
	{
	public:
		Hierarchy(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings);
		~Hierarchy() override = default;

		void Clear();
		void AttachActorNodeToParentNode(ECS::Actor& p_actor);
		void DetachActorNodeFromParentNode(ECS::Actor& p_actor);
		void AddActorByInstance(ECS::Actor& p_actor);
		void DeleteActorByInstance(ECS::Actor& p_actor);

	protected:
		void DrawContent() override;

	private:
		uint64_t m_destroyedListener;

		TreeNode* m_root;

		std::unordered_map<ECS::Actor*, TreeNode*> m_widgetActorLink;
	};
}
