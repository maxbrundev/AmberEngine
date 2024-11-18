#include "Amberpch.h"

#include "AmberEngine/Core/SceneSystem/Scene.h"

#include "AmberEngine/Core/ECS/Actor.h"

#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"
#include "AmberEngine/Core/ECS/Components/CLight.h"

AmberEngine::Eventing::Event<bool> AmberEngine::Core::SceneSystem::Scene::DirtyEvent;


AmberEngine::Core::SceneSystem::Scene::~Scene()
{
	std::for_each(m_actors.begin(), m_actors.end(), [](ECS::Actor* element)
	{
		delete element;
	});

	m_actors.clear();
}

void AmberEngine::Core::SceneSystem::Scene::Play()
{
	m_isPlaying = true;

	/* Wake up actors to allow them to react to OnEnable, OnDisable and OnDestroy, */
	std::for_each(m_actors.begin(), m_actors.end(), [](ECS::Actor * p_element) { p_element->SetSleeping(false); });

	std::for_each(m_actors.begin(), m_actors.end(), [](ECS::Actor * p_element) { if (p_element->IsActive()) p_element->OnAwake(); });
	std::for_each(m_actors.begin(), m_actors.end(), [](ECS::Actor * p_element) { if (p_element->IsActive()) p_element->OnEnable(); });
	std::for_each(m_actors.begin(), m_actors.end(), [](ECS::Actor * p_element) { if (p_element->IsActive()) p_element->OnStart(); });
}

bool AmberEngine::Core::SceneSystem::Scene::IsPlaying() const
{
	return m_isPlaying;
}

void AmberEngine::Core::SceneSystem::Scene::Update(float p_deltaTime)
{
	auto actors = m_actors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&ECS::Actor::OnUpdate), std::placeholders::_1, p_deltaTime));
}

void AmberEngine::Core::SceneSystem::Scene::FixedUpdate(float p_deltaTime)
{
	auto actors = m_actors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&ECS::Actor::OnFixedUpdate), std::placeholders::_1, p_deltaTime));
}

void AmberEngine::Core::SceneSystem::Scene::LateUpdate(float p_deltaTime)
{
	auto actors = m_actors;
	std::for_each(actors.begin(), actors.end(), std::bind(std::mem_fn(&ECS::Actor::OnLateUpdate), std::placeholders::_1, p_deltaTime));
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

	DirtyEvent.Invoke(true);

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

void AmberEngine::Core::SceneSystem::Scene::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	tinyxml2::XMLNode* sceneNode = p_doc.NewElement("scene");
	p_node->InsertEndChild(sceneNode);

	tinyxml2::XMLNode* actorsNode = p_doc.NewElement("actors");
	sceneNode->InsertEndChild(actorsNode);

	for (auto& actor : m_actors)
	{
		actor->OnSerialize(p_doc, actorsNode);
	}
}

void AmberEngine::Core::SceneSystem::Scene::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
	tinyxml2::XMLNode* actorsRoot = p_node->FirstChildElement("actors");

	if (actorsRoot)
	{
		tinyxml2::XMLElement* currentActor = actorsRoot->FirstChildElement("actor");

		int64_t maxID = 1;

		while (currentActor)
		{
			auto& actor = CreateActor();
			actor.OnDeserialize(p_doc, currentActor);
			maxID = std::max(actor.GetID() + 1, maxID);
			currentActor = currentActor->NextSiblingElement("actor");
		}

		m_availableID = maxID;

		/* We recreate the hierarchy of the scene by attaching children to their parents */
		for (auto actor : m_actors)
		{
			if (actor->GetParentID() > 0)
			{
				if (auto found = FindActorByID(actor->GetParentID()); found)
					actor->SetParent(*found);
			}
		}
	}
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
		*found = nullptr;

		m_actors.erase(found);

		return true;
	}

	return false;
}

AmberEngine::Core::ECS::Actor* AmberEngine::Core::SceneSystem::Scene::FindActorByID(int64_t p_id)
{
	auto result = std::find_if(m_actors.begin(), m_actors.end(), [p_id](ECS::Actor* element)
	{
		return element->GetID() == p_id;
	});

	if (result != m_actors.end())
		return *result;
	else
		return nullptr;
}

void AmberEngine::Core::SceneSystem::Scene::CollectGarbage()
{
	m_actors.erase(std::remove_if(m_actors.begin(), m_actors.end(), [this](ECS::Actor* element)
	{
		bool isGarbage = !element->IsAlive();
		if (isGarbage)
		{
			delete element;
		}
		return isGarbage;
	}), m_actors.end());
}

void AmberEngine::Core::SceneSystem::Scene::SetDirty(bool p_isDirty)
{
	m_isDirty = p_isDirty;
}

void AmberEngine::Core::SceneSystem::Scene::OnComponentAdded(ECS::Components::AComponent& p_compononent)
{
	if(auto result = dynamic_cast<ECS::Components::CModelRenderer*>(&p_compononent))
		m_fastAccessComponents.modelRenderers.push_back(result);

	if (auto result = dynamic_cast<ECS::Components::CCamera*>(&p_compononent))
		m_fastAccessComponents.cameras.push_back(result);

	if (auto result = dynamic_cast<ECS::Components::CLight*>(&p_compononent))
		m_fastAccessComponents.lights.push_back(result);
}

void AmberEngine::Core::SceneSystem::Scene::OnComponentRemoved(ECS::Components::AComponent& p_compononent)
{
	if (auto result = dynamic_cast<ECS::Components::CModelRenderer*>(&p_compononent))
		m_fastAccessComponents.modelRenderers.erase(std::remove(m_fastAccessComponents.modelRenderers.begin(), m_fastAccessComponents.modelRenderers.end(), result), m_fastAccessComponents.modelRenderers.end());

	if (auto result = dynamic_cast<ECS::Components::CCamera*>(&p_compononent))
		m_fastAccessComponents.cameras.erase(std::remove(m_fastAccessComponents.cameras.begin(), m_fastAccessComponents.cameras.end(), result), m_fastAccessComponents.cameras.end());

	if (auto result = dynamic_cast<ECS::Components::CLight*>(&p_compononent))
		m_fastAccessComponents.lights.erase(std::remove(m_fastAccessComponents.lights.begin(), m_fastAccessComponents.lights.end(), result), m_fastAccessComponents.lights.end());
}
