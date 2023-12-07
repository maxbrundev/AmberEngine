#include "Amberpch.h"

#include "AmberEngine/Core/SceneSystem/Scene.h"

#include "AmberEngine/Core/ECS/Actor.h"

#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"
#include "AmberEngine/Core/ECS/Components/CLight.h"

AmberEngine::Core::SceneSystem::Scene::Scene(std::string p_name) : m_name(std::move(p_name))
{
}

AmberEngine::Core::SceneSystem::Scene::~Scene()
{
	std::for_each(m_actors.begin(), m_actors.end(), [](ECS::Actor* element)
	{
		delete element;
	});

	m_actors.clear();
}

void AmberEngine::Core::SceneSystem::Scene::AddActor(ECS::Actor* p_actor)
{
	m_actors.push_back(p_actor);

	ECS::Actor& instance = *m_actors.back();

	if (const auto result = instance.GetComponent<ECS::Components::CModelRenderer>(); result != nullptr)
	{
		m_fastAccessComponents.modelRenderers.push_back(result);
	}

	if (const auto result = instance.GetComponent<ECS::Components::CLight>(); result != nullptr)
	{
		m_fastAccessComponents.lights.push_back(result);
	}

	instance.ComponentAddedEvent   += std::bind(&Scene::OnComponentAdded, this, std::placeholders::_1);
	instance.ComponentRemovedEvent += std::bind(&Scene::OnComponentRemoved, this, std::placeholders::_1);
}

AmberEngine::Core::ECS::Actor& AmberEngine::Core::SceneSystem::Scene::CreateActor()
{
	return CreateActor("New Actor");
}

AmberEngine::Core::ECS::Actor& AmberEngine::Core::SceneSystem::Scene::CreateActor(const std::string& p_name, const std::string& p_tag)
{
	m_actors.push_back(new ECS::Actor(m_availableID++, p_name, p_tag));

	ECS::Actor& instance = *m_actors.back();

	instance.ComponentAddedEvent += std::bind(&Scene::OnComponentAdded, this, std::placeholders::_1);
	instance.ComponentRemovedEvent += std::bind(&Scene::OnComponentRemoved, this, std::placeholders::_1);

	return instance;
}

std::vector<AmberEngine::Core::ECS::Actor*>& AmberEngine::Core::SceneSystem::Scene::GetActors()
{
	return m_actors;
}

const AmberEngine::Core::SceneSystem::Scene::FastAccessComponents& AmberEngine::Core::SceneSystem::Scene::GetFastAccessComponents() const
{
	return m_fastAccessComponents;
}

bool AmberEngine::Core::SceneSystem::Scene::DestroyActor(ECS::Actor& p_target)
{
	auto found = std::find_if(m_actors.begin(), m_actors.end(), [&p_target](ECS::Actor* element)
	{
		return element == &p_target;
	});

	if (found != m_actors.end())
	{
		delete *found;
		m_actors.erase(found);
		return true;
	}

	return false;
}

void AmberEngine::Core::SceneSystem::Scene::OnComponentAdded(ECS::Components::AComponent& p_compononent)
{
	if(auto result = dynamic_cast<ECS::Components::CModelRenderer*>(&p_compononent))
		m_fastAccessComponents.modelRenderers.push_back(result);

	if (auto result = dynamic_cast<ECS::Components::CLight*>(&p_compononent))
		m_fastAccessComponents.lights.push_back(result);
}

void AmberEngine::Core::SceneSystem::Scene::OnComponentRemoved(ECS::Components::AComponent& p_compononent)
{
	if (auto result = dynamic_cast<ECS::Components::CModelRenderer*>(&p_compononent))
		m_fastAccessComponents.modelRenderers.erase(std::remove(m_fastAccessComponents.modelRenderers.begin(), m_fastAccessComponents.modelRenderers.end(), result), m_fastAccessComponents.modelRenderers.end());

	if (auto result = dynamic_cast<ECS::Components::CLight*>(&p_compononent))
		m_fastAccessComponents.lights.erase(std::remove(m_fastAccessComponents.lights.begin(), m_fastAccessComponents.lights.end(), result), m_fastAccessComponents.lights.end());
}
