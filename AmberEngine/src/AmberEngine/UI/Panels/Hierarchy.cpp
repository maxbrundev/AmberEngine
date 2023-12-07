#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Hierarchy.h"

#include "AmberEngine/Core/Context.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"

AmberEngine::UI::Panels::Hierarchy::Hierarchy(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) :
APanelWindow(p_title, p_opened, p_panelSettings)
{
	AmberEngine::Core::ECS::Actor::CreatedEvent += std::bind(&Hierarchy::AddActorByInstance, this, std::placeholders::_1);
	m_destroyedListener = AmberEngine::Core::ECS::Actor::DestroyEvent += std::bind(&Hierarchy::DeleteActorByInstance, this, std::placeholders::_1);
	AmberEngine::Core::ECS::Actor::AttachEvent += std::bind(&Hierarchy::AttachActorNodeToParentNode, this, std::placeholders::_1);
	AmberEngine::Core::ECS::Actor::DettachEvent += std::bind(&Hierarchy::DetachActorNodeFromParentNode, this, std::placeholders::_1);

	Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().sceneManager.SceneUnloadEvent += std::bind(&Hierarchy::Clear, this);

	m_root = &CreateWidget<Widgets::TreeNode>("Root", true);
}

void AmberEngine::UI::Panels::Hierarchy::Clear()
{
	AmberEngine::Core::ECS::Actor::DestroyEvent.RemoveListener(m_destroyedListener);

	m_root->RemoveAllWidgets();

	m_widgetActorLink.clear();
}

void AmberEngine::UI::Panels::Hierarchy::AttachActorNodeToParentNode(AmberEngine::Core::ECS::Actor& p_actor)
{
	const auto actorWidget = m_widgetActorLink.find(&p_actor);

	if (actorWidget != m_widgetActorLink.end())
	{
		auto widget = actorWidget->second;

		if (widget->HasParent())
			widget->GetParent()->UnconsiderWidget(*widget);

		if (p_actor.HasParent())
		{
			const auto parentWidget = m_widgetActorLink.at(p_actor.GetParent());
			parentWidget->isLeaf = false;
			parentWidget->ConsiderWidget(*widget);
		}
	}
}

void AmberEngine::UI::Panels::Hierarchy::DetachActorNodeFromParentNode(AmberEngine::Core::ECS::Actor& p_actor)
{
	if (const auto actorWidget = m_widgetActorLink.find(&p_actor); actorWidget != m_widgetActorLink.end())
	{
		if (p_actor.HasParent() && p_actor.GetParent()->GetChildren().size() == 1)
		{
			if (const auto parentWidget = m_widgetActorLink.find(p_actor.GetParent()); parentWidget != m_widgetActorLink.end())
			{
				parentWidget->second->isLeaf = true;
			}
		}

		const auto widget = actorWidget->second;

		if (widget->HasParent())
			widget->GetParent()->UnconsiderWidget(*widget);

		m_root->ConsiderWidget(*widget);
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