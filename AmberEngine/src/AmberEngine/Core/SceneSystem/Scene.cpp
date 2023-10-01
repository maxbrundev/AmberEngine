#include "Amberpch.h"

#include "AmberEngine/Core/SceneSystem/Scene.h"

#include "AmberEngine/Core/ECS/Components/ModelComponent.h"
#include "AmberEngine/Core/ECS/Components/LightComponent.h"

AmberEngine::Core::SceneSystem::Scene::Scene(std::string p_name) : m_name(std::move(p_name))
{
}


void AmberEngine::Core::SceneSystem::Scene::AddActor(ECS::Actor* p_actor)
{
	const auto& actor = m_actors.emplace(p_actor->GetName(), p_actor);

	if(auto lightComponent = actor.first->second->GetComponent<ECS::Components::LightComponent>(); lightComponent != nullptr)
	{
		m_lights.emplace_back(lightComponent);
	}
}

void AmberEngine::Core::SceneSystem::Scene::DestroyActor(ECS::Actor*& p_actor)
{
	const auto it = m_actors.find(p_actor->GetName());
	
	if(it != m_actors.end())
	{
		delete p_actor;
		p_actor = nullptr;
	
		m_actors.erase(it->first);
	}
}

void AmberEngine::Core::SceneSystem::Scene::DrawAll(Renderer& p_renderer, Resources::Material* p_defaultMaterial) const
{
	for (const auto& actor : m_actors)
	{
		if(const auto modelComponent = actor.second->GetComponent<ECS::Components::ModelComponent>(); modelComponent != nullptr)
		{
			p_renderer.Draw(*modelComponent->GetModel(), &actor.second->GetTransform().GetWorldMatrix(), p_defaultMaterial);
		}
	}
}

void AmberEngine::Core::SceneSystem::Scene::Update(float p_deltaTime) const
{
	for (const auto& actor : m_actors)
	{
		// TODO: Find a better way to access lights.
		actor.second->Update(m_lights, p_deltaTime);
	}
}

void AmberEngine::Core::SceneSystem::Scene::Unload()
{
	for (auto& actor : m_actors)
	{
		delete actor.second;
		actor.second = nullptr;
	}

	SceneUnloadEvent.Invoke();

	m_actors.clear();
	m_lights.clear();
}

std::unordered_map<std::string, AmberEngine::ECS::Actor*>& AmberEngine::Core::SceneSystem::Scene::GetActors()
{
	return m_actors;
}

const std::vector<AmberEngine::ECS::Components::LightComponent*>& AmberEngine::Core::SceneSystem::Scene::GetLights()
{
	return m_lights;
}
