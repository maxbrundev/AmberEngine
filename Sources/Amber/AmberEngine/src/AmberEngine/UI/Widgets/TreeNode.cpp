#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/TreeNode.h"

#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/UI/Widgets/ContextualMenu.h"
#include "AmberEngine/UI/Widgets/InputText.h"
#include "AmberEngine/UI/Widgets/MenuItem.h"

AmberEngine::UI::Widgets::TreeNode::TreeNode(const std::string& p_name, bool p_arrowClickToOpen) :
name(p_name),
m_actorName(name),
m_actor(nullptr),
m_isArrowClickToOpen(p_arrowClickToOpen)
{
}

void AmberEngine::UI::Widgets::TreeNode::SetActor(AmberEngine::Core::ECS::Actor* p_actor)
{
	m_actor = p_actor;

	m_data = std::make_pair(m_actor, this);

	auto& test = CreateWidget<AmberEngine::UI::Widgets::ContextualMenu>();

	auto& testt = test.CreateWidget<MenuItem>("Delete");
	testt.ClickedEvent += [this]
	{
		EDITOR_EXEC(DestroyActor(std::ref(*m_actor)));
	};
	test.CreateWidget<MenuItem>("Duplicate");
	auto& renameMenu = test.CreateWidget<MenuList>("Rename to...");

	auto& nameEditor = renameMenu.CreateWidget<Widgets::InputText>("");

	nameEditor.selectAllOnClick = true;
	
	renameMenu.ClickedEvent += [this, &nameEditor]
	{
		nameEditor.content = m_actor->GetName();
	};
	
	nameEditor.EnterPressedEvent += [this](std::string p_newName)
	{
		if(!p_newName.empty())
			m_actor->SetName(p_newName);
	};
}

void AmberEngine::UI::Widgets::TreeNode::Update()
{
	if(m_actor != nullptr && m_actor->IsAlive())
	{
		m_actorName = m_actor->GetName();

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
			std::pair<AmberEngine::Core::ECS::Actor*, TreeNode*> data = *static_cast<std::pair<AmberEngine::Core::ECS::Actor*, TreeNode*>*>(payload->Data);

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

void AmberEngine::UI::Widgets::TreeNode::Open()
{
	m_isShouldOpen = true;
	m_isShouldClose = false;
}

void AmberEngine::UI::Widgets::TreeNode::Close()
{
	m_isShouldClose = true;
	m_isShouldOpen = false;
}

bool AmberEngine::UI::Widgets::TreeNode::IsOpened() const
{
	return m_isOpened;
}

void AmberEngine::UI::Widgets::TreeNode::DrawImplementation()
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

		DrawWidgets();

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