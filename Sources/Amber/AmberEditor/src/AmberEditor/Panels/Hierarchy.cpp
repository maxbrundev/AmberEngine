#include "Amberpch.h"

#include "AmberEditor/Panels/Hierarchy.h"

#include "AmberEditor/Utils/ActorCreationMenu.h"
#include "AmberEditor/Core/Context.h"
#include "AmberEditor/Core/EditorAction.h"
#include "AmberCore/SceneSystem/Scene.h"

#include "AmberTools/Global/ServiceLocator.h"
#include "AmberTools/Utils/String.h"
#include "AmberUI/Widgets/ContextualMenuItem.h"
#include "AmberUI/Widgets/ContextualMenuWindow.h"
#include "AmberUI/Widgets/InputText.h"
#include "AmberUI/Widgets/MenuItem.h"
#include "AmberUI/Widgets/MenuList.h"
#include "AmberUI/Widgets/Separator.h"
#include "AmberUI/Widgets/Text.h"

void AmberEditor::Panels::Hierarchy::ReorderActorNode(AmberCore::ECS::Actor& p_draggedActor, AmberUI::Widgets::TreeNode& p_draggedNode, AmberCore::ECS::Actor& p_targetActor, AmberUI::Widgets::TreeNode& p_targetNode, bool p_insertBefore)
{
	AmberCore::ECS::Actor* targetParent = p_targetActor.GetParent();

	if (targetParent == &p_draggedActor || (targetParent && targetParent->IsDescendantOf(&p_draggedActor)))
	{
		return;
	}

	if (p_draggedActor.GetParent() != targetParent)
	{
		if (targetParent)
		{
			p_draggedActor.SetParent(*targetParent);
		}
		else
		{
			p_draggedActor.RemoveParent();
		}
	}

	EDITOR_EXEC(DelayAction([&p_draggedNode, &p_targetNode, p_insertBefore]
	{
		AmberUI::WidgetContainer* container = p_targetNode.GetParent();

		if (!container || p_draggedNode.GetParent() != container)
		{
			return;
		}

		auto& widgets = container->GetWidgets();

		auto findIndex = [&widgets](AmberUI::Widgets::AWidget* p_widget)
		{
			for (size_t i = 0; i < widgets.size(); i++)
			{
				if (widgets[i].get() == p_widget)
				{
					return i;
				}
			}

			return widgets.size();
		};

		const size_t fromIndex = findIndex(&p_draggedNode);
		const size_t targetIndex = findIndex(&p_targetNode);

		if (fromIndex == widgets.size() || targetIndex == widgets.size())
		{
			return;
		}

		size_t insertIndex = p_insertBefore ? targetIndex : targetIndex + 1;

		if (fromIndex < insertIndex)
		{
			insertIndex--;
		}

		container->MoveWidgetIndex(fromIndex, insertIndex);
	}, 1));
}

void AmberEditor::Panels::Hierarchy::SetupActorTreeNode(AmberUI::Widgets::TreeNode& p_node, AmberCore::ECS::Actor* p_actor)
{
	auto& contextualMenu = p_node.CreateContextualMenu<AmberUI::Widgets::ContextualMenuItem>();

	if (p_actor)
	{
		auto& duplicateMenuItem = contextualMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Duplicate");
		duplicateMenuItem.ClickedEvent += [p_actor]
		{
			EDITOR_EXEC(DelayAction(EDITOR_BIND(DuplicateActor, std::ref(*p_actor), nullptr, true), 0));
		};

		auto& deleteMenuItem = contextualMenu.CreateWidget<AmberUI::Widgets::MenuItem>("Delete");
		deleteMenuItem.ClickedEvent += [p_actor]
		{
			EDITOR_EXEC(DestroyActor(std::ref(*p_actor)));
		};

		auto& renameMenuList = contextualMenu.CreateWidget<AmberUI::Widgets::MenuList>("Rename to...");

		auto& renameInputText = renameMenuList.CreateWidget<AmberUI::Widgets::InputText>("");
		renameInputText.SelectAllOnClick = true;
		renameMenuList.ClickedEvent += [p_actor, &renameInputText]
		{
			renameInputText.Content = p_actor->GetName();
		};

		renameInputText.EnterPressedEvent += [p_actor](std::string p_newName)
		{
			if (p_newName.empty())
				return;

			p_actor->SetName(std::move(p_newName));
		};
	}

	auto& createActor = contextualMenu.CreateWidget<AmberUI::Widgets::MenuList>("Create...");
	AmberEditor::Utils::ActorCreationMenu::GenerateActorCreationMenu(createActor, p_actor, [&p_node] { p_node.Open(); });

	p_node.DrawnEvent += [this, p_actor, &p_node]
	{
		if (p_actor != nullptr && p_actor->IsAlive())
		{
			p_node.Name = p_actor->GetName();

			ImGuiDragDropFlags src_flags = 0;
			src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;
			src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers;

			if (ImGui::BeginDragDropSource(src_flags))
			{
				if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
					ImGui::Text("Attach to..");

				std::pair<AmberCore::ECS::Actor*, AmberUI::Widgets::TreeNode*> data = std::make_pair(p_actor, &p_node);
				ImGui::SetDragDropPayload("Actor", &data, sizeof(data));
				ImGui::EndDragDropSource();
			}
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Actor", ImGuiDragDropFlags_AcceptBeforeDelivery | ImGuiDragDropFlags_AcceptNoDrawDefaultRect))
			{
				std::pair<AmberCore::ECS::Actor*, AmberUI::Widgets::TreeNode*> data = *static_cast<std::pair<AmberCore::ECS::Actor*, AmberUI::Widgets::TreeNode*>*>(payload->Data);

				const ImVec2 itemMin = ImGui::GetItemRectMin();
				const ImVec2 itemMax = ImGui::GetItemRectMax();
				const float itemHeight = itemMax.y - itemMin.y;
				const float mouseY = ImGui::GetMousePos().y;

				const bool canReorder = p_actor != nullptr && data.second != &p_node;
				const bool insertBefore = canReorder && mouseY < itemMin.y + itemHeight * 0.25f;
				const bool insertAfter = canReorder && (!p_node.IsOpened() || p_node.GetWidgets().empty()) && mouseY > itemMax.y - itemHeight * 0.25f;

				if (insertBefore || insertAfter)
				{
					const float lineY = insertBefore ? itemMin.y : itemMax.y;

					ImGui::GetWindowDrawList()->AddLine(ImVec2(itemMin.x, lineY), ImVec2(itemMax.x, lineY), ImGui::GetColorU32(ImGuiCol_DragDropTarget), 2.0f);

					if (payload->IsDelivery())
					{
						ReorderActorNode(*data.first, *data.second, *p_actor, p_node, insertBefore);
					}
				}
				else
				{
					ImGuiDragDropFlags target_flags = 0;

					if (p_actor != nullptr && p_actor->IsDescendantOf(data.first))
					{
						target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
					}

					if (ImGui::AcceptDragDropPayload("Actor", target_flags))
					{
						if (p_actor == nullptr)
						{
							data.first->RemoveParent();
						}
						else if (!p_actor->IsDescendantOf(data.first))
						{
							data.first->SetParent(*p_actor);
						}
					}
				}
			}

			ImGui::EndDragDropTarget();
		}
	};
}

AmberEditor::Panels::Hierarchy::Hierarchy(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings) :
AmberUI::Panels::APanelWindow(p_title, p_opened, p_panelSettings)
{
	AmberCore::ECS::Actor::CreatedEvent += [this](AmberCore::ECS::Actor& p_actor)
	{
		AddActorByInstance(p_actor);
	};

	AmberCore::ECS::Actor::DestroyEvent += [this](AmberCore::ECS::Actor& p_actor)
	{
		DeleteActorByInstance(p_actor);
	};

	AmberCore::ECS::Actor::AttachEvent += [this](AmberCore::ECS::Actor& p_actor, AmberCore::ECS::Actor& p_parentActor)
	{
		AttachActorNodeToParentNode(p_actor, p_parentActor);
	};

	AmberCore::ECS::Actor::DettachEvent += [this](AmberCore::ECS::Actor& p_actor, AmberCore::ECS::Actor* p_parentActor)
	{
		DetachActorNodeFromParentNode(p_actor, p_parentActor);
	};

	m_sceneNameText = &CreateWidget<AmberUI::Widgets::Text>();
	m_sceneNameText->Content = "New Scene";

	CreateWidget<AmberUI::Widgets::Separator>();
	AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::Context>().sceneManager.SceneUnloadEvent += std::bind(&Hierarchy::Clear, this);

	m_sceneNameChangedEventListerId = AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::Context>().sceneManager.CurrentSceneSourcePathChangedEvent.AddListener(std::bind(&Hierarchy::OnSceneNameChanged, this, std::placeholders::_1));
	AmberCore::SceneSystem::Scene::DirtyEvent += [&](bool cond)
	{

		if (*--m_sceneNameText->Content.end() == '*')
			return;

		m_sceneNameText->Content = m_sceneNameText->Content + "*";
	};

	m_root = &CreateWidget<AmberUI::Widgets::TreeNode>("Root", true);
	m_root->Open();
	SetupActorTreeNode(*m_root, nullptr);

	auto& contextualMenu = CreateContextualMenu<AmberUI::Widgets::ContextualMenuWindow>();
	auto& createActor = contextualMenu.CreateWidget<AmberUI::Widgets::MenuList>("Create...");
	AmberEditor::Utils::ActorCreationMenu::GenerateActorCreationMenu(createActor, nullptr, std::bind(&AmberUI::Widgets::TreeNode::Open, std::ref(m_root)));

	EDITOR_EVENT(ActorSelectionEvent) += [this]
	{
		for (auto& [actor, node] : m_widgetActorLink)
		{
			node->IsSelected = EDITOR_EXEC(IsActorInSelection(*actor));
		}
	};
}

AmberEditor::Panels::Hierarchy::~Hierarchy()
{
	AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::Context>().sceneManager.CurrentSceneSourcePathChangedEvent.RemoveListener(m_sceneNameChangedEventListerId);
}

void AmberEditor::Panels::Hierarchy::Clear()
{
	EDITOR_EXEC(UnselectActor());

	m_root->RemoveAllWidgets();

	m_widgetActorLink.clear();
}

void AmberEditor::Panels::Hierarchy::AttachActorNodeToParentNode(AmberCore::ECS::Actor& p_actor, AmberCore::ECS::Actor& p_parentActor)
{
	const auto actorWidget = m_widgetActorLink.find(&p_actor);

	if (actorWidget != m_widgetActorLink.end())
	{
		auto widget = actorWidget->second;

		auto parentWidget = m_widgetActorLink.at(p_actor.GetParent());
		parentWidget->IsLeaf = false;

		AmberUI::WidgetContainer::TransferWidget(*widget, *parentWidget);
	}
}

void AmberEditor::Panels::Hierarchy::DetachActorNodeFromParentNode(AmberCore::ECS::Actor& p_actor, AmberCore::ECS::Actor* p_parentActor)
{
	if (const auto actorWidget = m_widgetActorLink.find(&p_actor); actorWidget != m_widgetActorLink.end())
	{
		if (p_parentActor && p_parentActor->GetChildren().size() == 1)
		{
			if (const auto parentWidget = m_widgetActorLink.find(p_parentActor); parentWidget != m_widgetActorLink.end())
			{
				parentWidget->second->IsLeaf = true;
			}
		}

		const auto widget = actorWidget->second;

		if(widget->GetParent() != m_root)
			AmberUI::WidgetContainer::TransferWidget(*widget, *m_root);
	}
}

void AmberEditor::Panels::Hierarchy::AddActorByInstance(AmberCore::ECS::Actor& p_actor)
{
	AmberUI::Widgets::TreeNode& node = m_root->CreateWidget<AmberUI::Widgets::TreeNode>(p_actor.GetName(), true);
	node.IsLeaf = true;
	SetupActorTreeNode(node, &p_actor);
	m_widgetActorLink[&p_actor] = &node;

	node.ClickedEvent += [this, &p_actor]
	{
		SelectActor(p_actor);
	};
}

void AmberEditor::Panels::Hierarchy::DeleteActorByInstance(AmberCore::ECS::Actor& p_actor)
{
	if (const auto result = m_widgetActorLink.find(&p_actor); result != m_widgetActorLink.end())
	{
		if (result->second)
		{
			result->second->Destroy();
		}

		m_widgetActorLink.erase(result);
	}
}

void AmberEditor::Panels::Hierarchy::SelectActor(AmberCore::ECS::Actor& p_target)
{
	SelectActorEvent.Invoke(std::ref(p_target));
}

void AmberEditor::Panels::Hierarchy::OnSceneNameChanged(const std::string& p_sceneName)
{
	std::string sceneName;

	if (p_sceneName.empty())
	{
		sceneName = "New Scene";
	}
	else
	{
		sceneName = AmberTools::Utils::String::ExtractFileNameFromPath(p_sceneName);
		sceneName = AmberTools::Utils::String::RemoveExtensionFromFileName(sceneName);
	}

	m_sceneNameText->Content = sceneName;
}
