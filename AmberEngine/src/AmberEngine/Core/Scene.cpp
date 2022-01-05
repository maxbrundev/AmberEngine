#include "Amberpch.h"

#include "AmberEngine/Core/Scene.h"

#include "AmberEngine/Core/ECS/Components/ModelComponent.h"
#include "AmberEngine/Core/ECS/Components/LightComponent.h"

AmberEngine::Core::Scene::Scene(std::string p_name) : m_name(std::move(p_name))
{
}

AmberEngine::Core::Scene::~Scene()
{
	for (auto& actor : m_actors)
	{
		delete actor.second;
		actor.second = nullptr;
	}

	m_actors.clear();
}

void AmberEngine::Core::Scene::AddActor(ECS::Actor* p_actor, const std::string& p_name)
{
	m_actors.emplace(p_name, p_actor);
	p_actor->SetName(p_name);
}

void AmberEngine::Core::Scene::RemoveGameObject(ECS::Actor*& p_actor)
{
	m_actors.erase(m_actors.find(p_actor->GetName()));
}

void AmberEngine::Core::Scene::DrawAll(Core::Renderer& p_renderer)
{
	for (const auto& actor : m_actors)
	{
		if(actor.second->GetComponent<ECS::Components::ModelComponent>() != nullptr)
		{
			p_renderer.Draw(*actor.second->GetComponent<ECS::Components::ModelComponent>()->GetModel(), &actor.second->GetTransform().GetWorldMatrix());
		}
	}
}

void AmberEngine::Core::Scene::Update(float p_deltaTime)
{
	for (const auto& actor : m_actors)
	{
		actor.second->Update(p_deltaTime);
	}
}
