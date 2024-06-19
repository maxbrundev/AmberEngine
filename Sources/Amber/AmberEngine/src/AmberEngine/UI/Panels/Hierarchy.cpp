#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Hierarchy.h"

#include "AmberEngine/Core/ActorCreationMenu.h"
#include "AmberEngine/Core/Context.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/UI/Widgets/ContextualMenuWindow.h"
#include "AmberEngine/UI/Widgets/MenuList.h"

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

	Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().sceneManager.SceneUnloadEvent += std::bind(&Hierarchy::Clear, this);

	m_root = &CreateWidget<Widgets::TreeNode>("Root", true);
	m_root->Open();
	m_root->SetActor(nullptr);

	//TODO: Find a workaround to enable right click + ImGuiPopupFlags_NoOpenOverItems
	//auto& contextualMenu = CreateWidget<Widgets::ContextualMenuWindow>();
	//auto& createActor = contextualMenu.CreateWidget<Widgets::MenuList>("Create...");
	//Utils::ActorCreationMenu::GenerateActorCreationMenu(createActor, nullptr, std::bind(&Widgets::TreeNode::Open, m_root));
}

void AmberEngine::UI::Panels::Hierarchy::Clear()
{
	m_root->RemoveAllWidgets();

	m_widgetActorLink.clear();

	m_root->Open();
	m_root->SetActor(nullptr);
}

void AmberEngine::UI::Panels::Hierarchy::AttachActorNodeToParentNode(AmberEngine::Core::ECS::Actor& p_actor, AmberEngine::Core::ECS::Actor& p_parentActor)
{
	const auto actorWidget = m_widgetActorLink.find(&p_actor);

	if (actorWidget != m_widgetActorLink.end())
	{
		auto widget = actorWidget->second;

		const auto parentWidget = m_widgetActorLink.at(p_actor.GetParent());
		parentWidget->isLeaf = false;

		widget->GetParent()->TransferOwnership(*widget, *parentWidget);
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