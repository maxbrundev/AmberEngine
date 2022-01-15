#include "Amberpch.h"

#include "AmberEngine/Core/SceneSystem/Scene.h"

#include "AmberEngine/Core/ECS/Components/ModelComponent.h"
#include "AmberEngine/Core/ECS/Components/LightComponent.h"

AmberEngine::Core::SceneSystem::Scene::Scene(std::string p_name) : m_name(std::move(p_name))
{
}

AmberEngine::Core::SceneSystem::Scene::~Scene()
{
	for (auto& actor : m_actors)
	{
		delete actor.second;
		actor.second = nullptr;
	}

	m_actors.clear();
	m_lights.clear();
}

void AmberEngine::Core::SceneSystem::Scene::AddActor(ECS::Actor* p_actor, const std::string& p_name)
{
	auto actor = m_actors.emplace(p_name, p_actor);
	p_actor->SetName(p_name);

	if(auto lightComponent = actor.first->second->GetComponent<ECS::Components::LightComponent>(); lightComponent != nullptr)
	{
		m_lights.emplace_back(lightComponent);
	}
}

void AmberEngine::Core::SceneSystem::Scene::RemoveGameObject(ECS::Actor*& p_actor)
{
	m_actors.erase(m_actors.find(p_actor->GetName()));
}

void AmberEngine::Core::SceneSystem::Scene::DrawAll(Core::Renderer& p_renderer) const
{
	for (const auto& actor : m_actors)
	{
		if(actor.second->GetComponent<ECS::Components::ModelComponent>() != nullptr)
		{
			p_renderer.Draw(*actor.second->GetComponent<ECS::Components::ModelComponent>()->GetModel(), &actor.second->GetTransform().GetWorldMatrix());
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

std::unordered_map<std::string_view, AmberEngine::ECS::Actor*>& AmberEngine::Core::SceneSystem::Scene::GetActors()
{
	return m_actors;
}

const std::vector<AmberEngine::ECS::Components::LightComponent*>& AmberEngine::Core::SceneSystem::Scene::GetLights()
{
	return m_lights;
}
