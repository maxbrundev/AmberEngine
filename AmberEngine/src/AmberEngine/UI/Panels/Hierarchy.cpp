#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Hierarchy.h"

#include "AmberEngine/Core/Context.h"

#include "AmberEngine/Tools/Utils/ServiceLocator.h"

#include "AmberEngine/UI/TreeNode.h"

AmberEngine::UI::Hierarchy::Hierarchy(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) :
APanelWindow(p_title, p_opened, p_panelSettings)
{
	ECS::Actor::CreatedEvent += std::bind(&Hierarchy::AddActorByInstance, this, std::placeholders::_1);
	m_destroyedListener = ECS::Actor::DestroyEvent += std::bind(&Hierarchy::DeleteActorByInstance, this, std::placeholders::_1);
	ECS::Actor::AttachEvent += std::bind(&Hierarchy::AttachActorNodeToParentNode, this, std::placeholders::_1);
	ECS::Actor::DettachEvent += std::bind(&Hierarchy::DetachActorNodeFromParentNode, this, std::placeholders::_1);

	Utils::ServiceLocator::Get<Core::Context>().m_scene->SceneUnloadEvent += std::bind(&Hierarchy::Clear, this);

	m_root = new TreeNode("Root", true);
}

void AmberEngine::UI::Hierarchy::Clear()
{
	ECS::Actor::DestroyEvent.RemoveListener(m_destroyedListener);

	delete m_root;
	m_root = nullptr;

	m_widgetActorLink.clear();
}

void AmberEngine::UI::Hierarchy::AttachActorNodeToParentNode(ECS::Actor& p_actor)
{
	const auto actorWidget = m_widgetActorLink.find(&p_actor);

	if (actorWidget != m_widgetActorLink.end())
	{
		auto widget = actorWidget->second;

		if (widget->HasParent())
			widget->GetParent()->RemoveChild(*widget);

		if (p_actor.HasParent())
		{
			const auto parentWidget = m_widgetActorLink.at(p_actor.GetParent());
			parentWidget->isLeaf = false;
			parentWidget->SetChild(*widget);
		}
	}
}

void AmberEngine::UI::Hierarchy::DetachActorNodeFromParentNode(ECS::Actor& p_actor)
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
			widget->GetParent()->RemoveChild(*widget);

		m_root->SetChild(*widget);
	}
}

void AmberEngine::UI::Hierarchy::AddActorByInstance(ECS::Actor& p_actor)
{
	TreeNode& node = m_root->CreateTreeNodeChild(p_actor.GetName(), true);
	node.isLeaf = true;
	node.SetActor(&p_actor);
	m_widgetActorLink[&p_actor] = &node;
}

void AmberEngine::UI::Hierarchy::DeleteActorByInstance(ECS::Actor& p_actor)
{
	if (const auto result = m_widgetActorLink.find(&p_actor); result != m_widgetActorLink.end())
	{
		if (result->second)
		{
			m_root->RemoveChild(*result->second);

			delete result->second;
		}

		m_widgetActorLink.erase(result);
	}
}

void AmberEngine::UI::Hierarchy::DrawContent()
{
	m_root->Draw();
}