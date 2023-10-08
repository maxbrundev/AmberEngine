#include "Amberpch.h"

#include "AmberEngine/UI/TreeNode.h"

#include "AmberEngine/Core/ECS/Actor.h"

TreeNode::TreeNode(std::string_view p_name, bool p_arrowClickToOpen) : name(p_name),
                                                                       m_isArrowClickToOpen(p_arrowClickToOpen),
                                                                       m_parent(nullptr)
{
}

TreeNode::~TreeNode()
{
	RemoveAllChilds();
}

void TreeNode::SetActor(AmberEngine::ECS::Actor* p_actor)
{
	m_actor = p_actor;

	m_data = std::make_pair(m_actor, this);
}

void TreeNode::Update()
{
	if(m_actor != nullptr)
	{
		ImGuiDragDropFlags src_flags = 0;
		src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
		src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging

		if (ImGui::BeginDragDropSource(src_flags))
		{
			if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
				ImGui::Text("Attach to..");

			ImGui::SetDragDropPayload("Actor", &m_data, sizeof(m_data));
			ImGui::EndDragDropSource();
		}
	}

	if (ImGui::BeginDragDropTarget())
	{
		if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Actor", ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect))
		{
			std::pair<AmberEngine::ECS::Actor*, TreeNode*> data = *static_cast<std::pair<AmberEngine::ECS::Actor*, TreeNode*>*>(payload->Data);

			if (m_actor != nullptr && m_actor->IsDescendantOf(data.first))
			{
				target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
			}
			else
			{
				target_flags = 0;
			}

			if (ImGui::AcceptDragDropPayload("Actor", target_flags))
			{
				if(m_actor == nullptr)
				{
					data.first->RemoveParent();
				}
				else if (m_actor->IsDescendantOf(data.first))
				{
					return;
				}

				if(m_actor != nullptr)
					data.first->SetParent(*m_actor);
			}
		}

		ImGui::EndDragDropTarget();
	}
}

void TreeNode::Open()
{
	m_isShouldOpen = true;
	m_isShouldClose = false;
}

void TreeNode::Close()
{
	m_isShouldClose = true;
	m_isShouldOpen = false;
}

TreeNode& TreeNode::CreateTreeNodeChild(std::string_view p_name, bool p_arrowClickToOpen)
{
	m_childs.emplace_back(new TreeNode(p_name, p_arrowClickToOpen));
	TreeNode& child = *m_childs.back();
	child.SetParent(this);

	return child;
}

bool TreeNode::IsOpened() const
{
	return m_isOpened;
}

void TreeNode::SetParent(TreeNode* p_parent)
{
	m_parent = p_parent;
}

void TreeNode::SetChild(TreeNode& p_child)
{
	m_childs.emplace_back(&p_child);
	p_child.SetParent(this);
}

void TreeNode::RemoveChild(TreeNode& p_child)
{
	const auto found = std::find_if(m_childs.begin(), m_childs.end(), [&p_child] (const TreeNode* childIterator)
	{
		return childIterator == &p_child;
	});

	if(found != m_childs.end())
	{
		p_child.SetParent(nullptr);
		m_childs.erase(found);
	}
}

void TreeNode::RemoveAllChilds()
{
	std::for_each(m_childs.begin(), m_childs.end(), [](TreeNode* pair)
	{
		delete pair;

		pair = nullptr;
	});

	m_childs.clear();
}

void TreeNode::Draw()
{
	const bool prevOpened = m_isOpened;

	if (m_isShouldOpen)
	{
		ImGui::SetNextTreeNodeOpen(true);
		m_isShouldOpen = false;
	}
	else if (m_isShouldClose)
	{
		ImGui::SetNextTreeNodeOpen(false);
		m_isShouldClose = false;
	}

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
	if (m_isArrowClickToOpen) flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	if (isSelected)			flags |= ImGuiTreeNodeFlags_Selected;
	if (isLeaf)				flags |= ImGuiTreeNodeFlags_Leaf;

	const bool opened = ImGui::TreeNodeEx(name.c_str(), flags);

	if (ImGui::IsItemClicked() && (ImGui::GetMousePos().x - ImGui::GetItemRectMin().x) > ImGui::GetTreeNodeToLabelSpacing())
	{
		ClickedEvent.Invoke();

		if (ImGui::IsMouseDoubleClicked(0))
		{
			DoubleClickedEvent.Invoke();
		}
	}

	if (opened)
	{
		if (!prevOpened)
			OpenedEvent.Invoke();

		m_isOpened = true;

		Update();

		for (const auto child : m_childs)
		{
			child->Draw();
		}

		ImGui::TreePop();
	}
	else
	{
		if (prevOpened)
			ClosedEvent.Invoke();

		m_isOpened = false;

		Update();
	}
}

bool TreeNode::HasParent() const
{
	return m_parent != nullptr;
}

TreeNode* TreeNode::GetParent() const
{
	return m_parent;
}