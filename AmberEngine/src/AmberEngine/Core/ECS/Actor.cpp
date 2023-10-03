#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Actor.h"

#include "AmberEngine/Core/ECS/Components/ModelComponent.h"

AmberEngine::Eventing::Event<AmberEngine::ECS::Actor&> AmberEngine::ECS::Actor::CreatedEvent;
AmberEngine::Eventing::Event<AmberEngine::ECS::Actor&> AmberEngine::ECS::Actor::DestroyEvent;
AmberEngine::Eventing::Event<AmberEngine::ECS::Actor&, AmberEngine::ECS::Actor&> AmberEngine::ECS::Actor::AttachEvent;
AmberEngine::Eventing::Event<AmberEngine::ECS::Actor&> AmberEngine::ECS::Actor::DettachEvent;

AmberEngine::ECS::Actor::Actor(std::string p_name) : m_name(std::move(p_name))
{
	CreatedEvent.Invoke(*this);
}

AmberEngine::ECS::Actor::~Actor()
{
	for (auto& component : m_components)
	{
		delete component;
		component = nullptr;
	}

	m_components.clear();

	std::vector<Actor*> toDetach = m_children;

	for (const auto child : toDetach)
	{
		child->RemoveParent();
	}

	toDetach.clear();
	m_children.clear();

	RemoveParent();

	DestroyEvent.Invoke(*this);
}

void AmberEngine::ECS::Actor::Update(const std::vector<Components::LightComponent*>& p_lights, float p_deltaTime) const
{
	for (const auto component : m_components)
	{
		component->Update(p_deltaTime);
	}
}

void AmberEngine::ECS::Actor::SetName(std::string p_name)
{
	m_name = std::move(p_name);
}

void AmberEngine::ECS::Actor::SetParent(Actor& p_parent)
{
	RemoveParent();

	m_parent = &p_parent;
	m_transform.SetParent(p_parent.GetTransform());
	p_parent.m_children.push_back(this);

	AttachEvent.Invoke(*this, p_parent);
}

void AmberEngine::ECS::Actor::RemoveParent()
{
	DettachEvent.Invoke(*this);

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

	m_transform.RemoveParent();
}

bool AmberEngine::ECS::Actor::HasParent() const
{
	return m_parent != nullptr;
}

bool AmberEngine::ECS::Actor::HasChildren() const
{
	return !m_children.empty();
}

std::string AmberEngine::ECS::Actor::GetName()
{
	return m_name;
}

AmberEngine::ECS::Actor* AmberEngine::ECS::Actor::GetParent() const
{
	return m_parent;
}

std::vector<AmberEngine::ECS::Actor*>& AmberEngine::ECS::Actor::GetChildren()
{
	return m_children;
}

AmberEngine::Maths::Transform& AmberEngine::ECS::Actor::GetTransform()
{
	return m_transform;
}
