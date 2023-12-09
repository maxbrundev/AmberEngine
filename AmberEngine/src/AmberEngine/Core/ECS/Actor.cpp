#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Actor.h"

AmberEngine::Eventing::Event<AmberEngine::Core::ECS::Actor&> AmberEngine::Core::ECS::Actor::CreatedEvent;
AmberEngine::Eventing::Event<AmberEngine::Core::ECS::Actor&> AmberEngine::Core::ECS::Actor::DestroyEvent;
AmberEngine::Eventing::Event<AmberEngine::Core::ECS::Actor&, AmberEngine::Core::ECS::Actor&> AmberEngine::Core::ECS::Actor::AttachEvent;
AmberEngine::Eventing::Event<AmberEngine::Core::ECS::Actor&, AmberEngine::Core::ECS::Actor*> AmberEngine::Core::ECS::Actor::DettachEvent;

AmberEngine::Core::ECS::Actor::Actor(int64_t p_actorID, const std::string& p_name, const std::string & p_tag) : m_actorID(p_actorID),
m_name(p_name),
m_tag(p_tag),
transform(AddComponent<Components::CTransform>())
{
	CreatedEvent.Invoke(*this);
}

AmberEngine::Core::ECS::Actor::~Actor()
{
	DestroyEvent.Invoke(*this);

	std::vector<Actor*> toDetach = m_children;

	for (const auto child : toDetach)
	{
		child->RemoveParent();
	}

	toDetach.clear();
	m_children.clear();

	RemoveParent();

	std::for_each(m_components.begin(), m_components.end(), [&](const std::shared_ptr<Components::AComponent>& p_component) { ComponentRemovedEvent.Invoke(*p_component); });

	m_components.clear();
}

bool AmberEngine::Core::ECS::Actor::RemoveComponent(Components::AComponent& p_component)
{
	for (auto it = m_components.begin(); it != m_components.end(); ++it)
	{
		if (it->get() == &p_component)
		{
			m_components.erase(it);
			return true;
		}
	}

	return false;
}

std::vector<std::shared_ptr<AmberEngine::Core::ECS::Components::AComponent>>& AmberEngine::Core::ECS::Actor::GetComponents()
{
	return m_components;
}

void AmberEngine::Core::ECS::Actor::SetName(std::string p_name)
{
	m_name = std::move(p_name);
}

void AmberEngine::Core::ECS::Actor::SetParent(Actor& p_parent)
{
	RemoveParent();

	m_parent = &p_parent;
	transform.SetParent(p_parent.transform);
	p_parent.m_children.push_back(this);

	AttachEvent.Invoke(*this, p_parent);
}

void AmberEngine::Core::ECS::Actor::RemoveParent()
{
	DettachEvent.Invoke(*this, m_parent);

	if (m_parent)
	{
		auto predicate = [this](const Actor* p_element)
		{
			return p_element == this;
		};

		m_parent->m_children.erase(std::remove_if(m_parent->m_children.begin(), m_parent->m_children.end(),
		                                          std::ref(predicate)));
	}

	m_parent = nullptr;

	transform.RemoveParent();
}

bool AmberEngine::Core::ECS::Actor::HasParent() const
{
	return m_parent != nullptr;
}

bool AmberEngine::Core::ECS::Actor::HasChildren() const
{
	return !m_children.empty();
}

bool AmberEngine::Core::ECS::Actor::IsDescendantOf(const Actor* p_actor) const
{
	const Actor* currentParentActor = m_parent;

	while (currentParentActor != nullptr)
	{
		if (currentParentActor == p_actor)
		{
			return true;
		}
		currentParentActor = currentParentActor->GetParent();
	}

	return false;
}

const std::string& AmberEngine::Core::ECS::Actor::GetName() const
{
	return m_name;
}

const std::string& AmberEngine::Core::ECS::Actor::GetTag() const
{
	return m_tag;
}

int64_t AmberEngine::Core::ECS::Actor::GetID() const
{
	return m_actorID;
}

AmberEngine::Core::ECS::Actor* AmberEngine::Core::ECS::Actor::GetParent() const
{
	return m_parent;
}

void AmberEngine::Core::ECS::Actor::SetActive(bool p_active)
{
	if (p_active != m_active)
	{
		m_active = p_active;
	}
}

bool AmberEngine::Core::ECS::Actor::IsSelfActive() const
{
	return m_active;
}

bool AmberEngine::Core::ECS::Actor::IsActive() const
{
	return m_active && (m_parent ? m_parent->IsActive() : true);
}

std::vector<AmberEngine::Core::ECS::Actor*>& AmberEngine::Core::ECS::Actor::GetChildren()
{
	return m_children;
}