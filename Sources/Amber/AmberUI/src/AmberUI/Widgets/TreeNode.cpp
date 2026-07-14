#include "Amberpch.h"

#include "AmberUI/Widgets/TreeNode.h"

AmberUI::Widgets::TreeNode::TreeNode(const std::string& p_name, bool p_arrowClickToOpen) :
Name(p_name),
m_isArrowClickToOpen(p_arrowClickToOpen)
{
	m_autoExecuteDragDrop = false;
}

void AmberUI::Widgets::TreeNode::Open()
{
	m_isShouldOpen = true;
	m_isShouldClose = false;
}

void AmberUI::Widgets::TreeNode::Close()
{
	m_isShouldClose = true;
	m_isShouldOpen = false;
}

bool AmberUI::Widgets::TreeNode::IsOpened() const
{
	return m_isOpened;
}

void AmberUI::Widgets::TreeNode::DrawImplementation()
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
	if (IsSelected)			flags |= ImGuiTreeNodeFlags_Selected;
	if (IsLeaf)				flags |= ImGuiTreeNodeFlags_Leaf;
	flags |= ImGuiTreeNodeFlags_AllowItemOverlap;

	const bool opened = ImGui::TreeNodeEx(Name.c_str(), flags);

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

		DrawnEvent.Invoke();
		DrawContextualMenu();
		DrawDragDrop();

		DrawWidgets();

		ImGui::TreePop();

	}
	else
	{
		if (prevOpened)
			ClosedEvent.Invoke();

		m_isOpened = false;

		DrawnEvent.Invoke();
		DrawContextualMenu();
		DrawDragDrop();
	}
}
