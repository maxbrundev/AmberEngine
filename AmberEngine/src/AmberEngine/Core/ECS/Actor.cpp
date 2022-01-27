#include "Amberpch.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/ECS/Components/ModelComponent.h"

#include "AmberEngine/Resources/Shader.h"

AmberEngine::Eventing::Event<AmberEngine::ECS::Actor&> AmberEngine::ECS::Actor::CreatedEvent;
AmberEngine::Eventing::Event<AmberEngine::ECS::Actor&> AmberEngine::ECS::Actor::DestroyEvent;

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

	m_transform.RemoveParent();

	DestroyEvent.Invoke(*this);
}

void AmberEngine::ECS::Actor::Update(const std::vector<ECS::Components::LightComponent*>& p_lights, float p_deltaTime)
{
	for (const auto component : m_components)
	{
		component->Update(p_deltaTime);
	}

	if(const auto modelComponent = GetComponent<ECS::Components::ModelComponent>(); modelComponent != nullptr)
	{
		const auto shader = modelComponent->GetModel()->GetShader();
		shader->Bind();

		for(auto& light : p_lights)
		{
			auto lightData = light->GetLightData();
			shader->SetUniformVec3("light.direction", light->owner.GetTransform().GetWorldForward());
			shader->SetUniformVec3("light.color", lightData.color);
			shader->SetUniform1f("light.intensity", lightData.intensity);
		}

		shader->Unbind();
	}
}

void AmberEngine::ECS::Actor::RemoveParent()
{
	if (m_parent)
	{
		m_parent->m_children.erase(std::remove_if(m_parent->m_children.begin(), m_parent->m_children.end(), [this](Actor* p_element)
		{
			return p_element == this;
		}));
	}

	m_parent = nullptr;

	m_transform.RemoveParent();
}

void AmberEngine::ECS::Actor::SetName(std::string p_name)
{
	m_name = std::move(p_name);
}

void AmberEngine::ECS::Actor::SetParent(Actor& p_parent)
{
	m_transform.RemoveParent();

	m_parent = &p_parent;
	m_transform.SetParent(p_parent.GetTransform());
	p_parent.m_children.push_back(this);
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
