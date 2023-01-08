#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Hierarchy.h"

#include "AmberEngine/Data/Constants.h"

uint64_t AmberEngine::UI::Hierarchy::__TREENODE_ID = 0;

AmberEngine::UI::Hierarchy::Hierarchy() : m_name(Data::Constants::EDITOR_PANEL_HIERARCHY_NAME)
{
	ECS::Actor::CreatedEvent += std::bind(&Hierarchy::AddActorByInstance, this, std::placeholders::_1);
	m_destroyedListener = ECS::Actor::DestroyEvent += std::bind(&Hierarchy::DeleteActorByInstance, this, std::placeholders::_1);

	m_rootID = "##" + std::to_string(__TREENODE_ID++);
}

AmberEngine::UI::Hierarchy::~Hierarchy()
{
	ECS::Actor::DestroyEvent.RemoveListener(m_destroyedListener);

	m_actors.clear();
}

void AmberEngine::UI::Hierarchy::AddActorByInstance(ECS::Actor& p_actor)
{
	const std::string actorTreeNodeID = "##" + std::to_string(__TREENODE_ID++);
	m_actors[&p_actor] = actorTreeNodeID;
}

void AmberEngine::UI::Hierarchy::DeleteActorByInstance(ECS::Actor& p_actor)
{
	if(const auto result = m_actors.find(&p_actor); result != m_actors.end())
	{
		if (result->first)
		{
			m_actors.erase(result->first);
		}
	}
}

// TODO: Clean TreeNodeEx usage by creating a dedicated class.
void AmberEngine::UI::Hierarchy::Draw() const
{
	ImGui::Begin(m_name.c_str());

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_Selected;
	flags |= ImGuiTreeNodeFlags_DefaultOpen;

	if (ImGui::TreeNodeEx((Data::Constants::EDITOR_TREE_NODE_HIERARCHY_LABEL_ROOT + m_rootID).c_str(), flags))
	{
		for (auto& actor : m_actors)
		{
			if (actor.first->GetTransform().HasParent())
			{
				continue;
			}

			flags = actor.first->HasChildren() ? ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_Leaf;

			if (ImGui::TreeNodeEx((actor.first->GetName() + actor.second).c_str(), flags))
			{
				uint64_t childID = 0;

				for(const auto child : actor.first->GetChildren())
				{
					if (ImGui::TreeNodeEx((child->GetName() + actor.second + std::to_string(childID++)).c_str(), ImGuiTreeNodeFlags_Leaf))
					{
						ImGui::TreePop();
					}
				}

				ImGui::TreePop();
			}
		}

		ImGui::TreePop();
	}

	ImGui::End();
}
