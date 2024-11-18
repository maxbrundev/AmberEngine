#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Hierarchy.h"

#include "AmberEngine/Core/ActorCreationMenu.h"
#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/Core/SceneSystem/Scene.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/Tools/Utils/String.h"
#include "AmberEngine/UI/Widgets/ContextualMenuWindow.h"
#include "AmberEngine/UI/Widgets/MenuList.h"
#include "AmberEngine/UI/Widgets/Separator.h"
#include "AmberEngine/UI/Widgets/Text.h"

AmberEngine::UI::Panels::Hierarchy::Hierarchy(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) :
APanelWindow(p_title, p_opened, p_panelSettings)
{
	AmberEngine::Core::ECS::Actor::CreatedEvent += [this](AmberEngine::Core::ECS::Actor& p_actor)
	{
		m_callbackQueue.push([this, &p_actor] { this->AddActorByInstance(p_actor); });
	};

	AmberEngine::Core::ECS::Actor::DestroyEvent += [this](AmberEngine::Core::ECS::Actor& p_actor)
	{
		m_callbackQueue.push([this, &p_actor] { this->DeleteActorByInstance(p_actor); });
	};

	AmberEngine::Core::ECS::Actor::AttachEvent += [this](AmberEngine::Core::ECS::Actor& p_actor, AmberEngine::Core::ECS::Actor& p_parentActor)
	{
		m_callbackQueue.push([this, &p_actor, &p_parentActor] { this->AttachActorNodeToParentNode(p_actor, p_parentActor); });
	};

	AmberEngine::Core::ECS::Actor::DettachEvent += [this](AmberEngine::Core::ECS::Actor& p_actor, AmberEngine::Core::ECS::Actor* p_parentActor)
	{
		m_callbackQueue.push([this, &p_actor, p_parentActor] { this->DetachActorNodeFromParentNode(p_actor, p_parentActor); });
	};

	m_sceneNameText = &CreateWidget<Widgets::Text>();
	m_sceneNameText->content = "New Scene";

	CreateWidget<Widgets::Separator>();
	Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().sceneManager.SceneUnloadEvent += std::bind(&Hierarchy::Clear, this);

	m_sceneNameChangedEventListerId = Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().sceneManager.CurrentSceneSourcePathChangedEvent.AddListener(std::bind(&Hierarchy::OnSceneNameChanged, this, std::placeholders::_1));
	AmberEngine::Core::SceneSystem::Scene::DirtyEvent += [&](bool cond)
	{

		if (*--m_sceneNameText->content.end() == '*')
			return;

		m_sceneNameText->content = m_sceneNameText->content + "*";
	};

	m_root = &CreateWidget<Widgets::TreeNode>("Root", true);
	m_root->Open();
	m_root->SetActor(nullptr);

	auto& contextualMenu = CreateContextualMenu<Widgets::ContextualMenuWindow>();
	auto& createActor = contextualMenu.CreateWidget<Widgets::MenuList>("Create...");
	Utils::ActorCreationMenu::GenerateActorCreationMenu(createActor, nullptr, std::bind(&Widgets::TreeNode::Open, std::ref(m_root)));
}

AmberEngine::UI::Panels::Hierarchy::~Hierarchy()
{
	Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().sceneManager.CurrentSceneSourcePathChangedEvent.RemoveListener(m_sceneNameChangedEventListerId);
}

void AmberEngine::UI::Panels::Hierarchy::Clear()
{
	EDITOR_EXEC(UnselectActor());

	m_root->RemoveAllWidgets();

	m_widgetActorLink.clear();
}

void AmberEngine::UI::Panels::Hierarchy::AttachActorNodeToParentNode(AmberEngine::Core::ECS::Actor& p_actor, AmberEngine::Core::ECS::Actor& p_parentActor)
{
	const auto actorWidget = m_widgetActorLink.find(&p_actor);

	if (actorWidget != m_widgetActorLink.end())
	{
		auto widget = actorWidget->second;

		auto parentWidget = m_widgetActorLink.at(p_actor.GetParent());
		parentWidget->isLeaf = false;

		widget->GetParent()->TransferOwnership(*widget, *parentWidget);

		widget->UpEvent += [this, parentWidget](Widgets::TreeNode* tree_node)
		{
			auto it = std::find_if(parentWidget->m_widgets.begin(), parentWidget->m_widgets.end(),
				[tree_node](const std::unique_ptr<Widgets::AWidget>& widget_ptr) {
				return widget_ptr.get() == tree_node;
			});

			if (it != parentWidget->m_widgets.end())
			{
				size_t index = std::distance(parentWidget->m_widgets.begin(), it);
				// index now holds the position of tree_node in m_widgets, if found
				std::cout << "Index of TreeNode in vector is: " << index << std::endl;

				if (index > 0)
				{
					parentWidget->MoveWidgetIndex(index, index - 1);
				}
			}
			else
			{
				std::cout << "TreeNode not found in the vector." << std::endl;
			}
		};

		widget->DownEvent += [this, parentWidget, &widget](Widgets::TreeNode* tree_node)
		{
			auto it = std::find_if(parentWidget->m_widgets.begin(), parentWidget->m_widgets.end(),
				[tree_node](const std::unique_ptr<Widgets::AWidget>& widget_ptr) {
				return widget_ptr.get() == tree_node;
			});
		
			if (it != parentWidget->m_widgets.end())
			{
				size_t index = std::distance(parentWidget->m_widgets.begin(), it);
				// index now holds the position of tree_node in m_widgets, if found
				std::cout << "Index of TreeNode in vector is: " << index << std::endl;
		
				if (index < parentWidget->m_widgets.size())
				{
					parentWidget->MoveWidgetIndex(index, index + 1);
				}
			}
			else
			{
				std::cout << "TreeNode not found in the vector." << std::endl;
			}
		};
	}
}

void AmberEngine::UI::Panels::Hierarchy::DetachActorNodeFromParentNode(AmberEngine::Core::ECS::Actor& p_actor, AmberEngine::Core::ECS::Actor* p_parentActor)
{
	if (const auto actorWidget = m_widgetActorLink.find(&p_actor); actorWidget != m_widgetActorLink.end())
	{
		if (p_parentActor && p_parentActor->GetChildren().size() <= 0)
		{
			if (const auto parentWidget = m_widgetActorLink.find(p_parentActor); parentWidget != m_widgetActorLink.end())
			{
				parentWidget->second->isLeaf = true;
			}
		}

		const auto widget = actorWidget->second;

		if(widget->GetParent() != m_root)
			widget->GetParent()->TransferOwnership(*widget, *m_root);
	}
}

void AmberEngine::UI::Panels::Hierarchy::AddActorByInstance(AmberEngine::Core::ECS::Actor& p_actor)
{
	Widgets::TreeNode& node = m_root->CreateWidget<Widgets::TreeNode>(p_actor.GetName(), true);
	node.isReorder = true;
	node.UpEvent += [&](Widgets::TreeNode* tree_node)
	{
		auto it = std::find_if(m_root->m_widgets.begin(), m_root->m_widgets.end(),
			[tree_node](const std::unique_ptr<Widgets::AWidget>& widget_ptr) {
			return widget_ptr.get() == tree_node;
		});

		if (it != m_root->m_widgets.end())
		{
			size_t index = std::distance(m_root->m_widgets.begin(), it);
			// index now holds the position of tree_node in m_widgets, if found
			std::cout << "Index of TreeNode in vector is: " << index << std::endl;

			if(index > 0)
			{
				m_root->MoveWidgetIndex(index, index - 1);
			}
		}
		else
		{
			std::cout << "TreeNode not found in the vector." << std::endl;
		}
	};

	node.DownEvent += [&](Widgets::TreeNode* tree_node)
	{
		auto it = std::find_if(m_root->m_widgets.begin(), m_root->m_widgets.end(),
			[tree_node](const std::unique_ptr<Widgets::AWidget>& widget_ptr) {
			return widget_ptr.get() == tree_node;
		});

		if (it != m_root->m_widgets.end())
		{
			size_t index = std::distance(m_root->m_widgets.begin(), it);
			// index now holds the position of tree_node in m_widgets, if found
			std::cout << "Index of TreeNode in vector is: " << index << std::endl;

			if (index < m_root->m_widgets.size())
			{
				m_root->MoveWidgetIndex(index, index + 1);
			}
		}
		else
		{
			std::cout << "TreeNode not found in the vector." << std::endl;
		}
	};
	node.isLeaf = true;
	node.SetActor(&p_actor);
	m_widgetActorLink[&p_actor] = &node;

	node.ClickedEvent += std::bind(&Hierarchy::SelectActor, this, std::ref(p_actor));
}

void AmberEngine::UI::Panels::Hierarchy::DeleteActorByInstance(AmberEngine::Core::ECS::Actor& p_actor)
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

void AmberEngine::UI::Panels::Hierarchy::SelectActor(AmberEngine::Core::ECS::Actor& p_target)
{
	SelectActorEvent.Invoke(std::ref(p_target));
}

void AmberEngine::UI::Panels::Hierarchy::OnSceneNameChanged(const std::string& p_sceneName)
{
	std::string sceneName;

	if (p_sceneName.empty())
	{
		sceneName = "New Scene";
	}
	else
	{
		sceneName = AmberEngine::Tools::Utils::String::ExtractFileNameFromPath(p_sceneName);
		sceneName = AmberEngine::Tools::Utils::String::RemoveExtensionFromFileName(sceneName);
	}

	m_sceneNameText->content = sceneName;
}
