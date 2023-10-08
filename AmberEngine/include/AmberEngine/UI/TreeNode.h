#pragma once

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::ECS
{
	class Actor;
}

class TreeNode
{
public:
	TreeNode(std::string_view p_name, bool p_arrowClickToOpen = false);
	~TreeNode();

	void SetActor(AmberEngine::ECS::Actor* p_actor);

	void Update();

	void Open();

	void Close();

	TreeNode& CreateTreeNodeChild(std::string_view p_name, bool p_arrowClickToOpen = false);

	bool IsOpened() const;

	void SetParent(TreeNode* p_parent);

	void SetChild(TreeNode& p_child);

	void RemoveChild(TreeNode& p_child);

	void RemoveAllChilds();

	void Draw();

	bool HasParent() const;

	TreeNode* GetParent() const;

public:
	std::string name;
	bool isSelected = false;
	bool isLeaf     = false;

	AmberEngine::Eventing::Event<> ClickedEvent;
	AmberEngine::Eventing::Event<> DoubleClickedEvent;
	AmberEngine::Eventing::Event<> OpenedEvent;
	AmberEngine::Eventing::Event<> ClosedEvent;

	AmberEngine::ECS::Actor* m_actor;

private:
	bool m_isArrowClickToOpen = false;
	bool m_isShouldOpen       = false;
	bool m_isShouldClose      = false;
	bool m_isOpened           = false;

	TreeNode* m_parent;

	std::vector<TreeNode*> m_childs;

	std::pair<AmberEngine::ECS::Actor*, TreeNode*> m_data;

	ImGuiDragDropFlags target_flags = 0;
};
