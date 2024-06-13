#include "Amberpch.h"

#include "AmberEngine/Core/SceneSystem/SceneManager.h"

#include "AmberEngine/Core/SceneSystem/Scene.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/ECS/Components/CDirectionalLight.h"

AmberEngine::Core::SceneSystem::SceneManager::SceneManager(const std::string& p_sceneRootFolder)
{
	LoadEmptyScene();
}

AmberEngine::Core::SceneSystem::SceneManager::~SceneManager()
{
	UnloadCurrentScene();
}

void AmberEngine::Core::SceneSystem::SceneManager::LoadEmptyScene()
{
	UnloadCurrentScene();

	m_currentScene = new Scene("Sample Scene");

	SceneLoadEvent.Invoke();
}

void AmberEngine::Core::SceneSystem::SceneManager::LoadEmptyLightedScene()
{
	UnloadCurrentScene();

	m_currentScene = new Scene("Sample Scene");

	SceneLoadEvent.Invoke();

	auto& directionalLight = m_currentScene->CreateActor("Directional Light");
	directionalLight.AddComponent<ECS::Components::CDirectionalLight>().SetIntensity(0.75f);
	directionalLight.transform.SetLocalPosition({ 0.0f, 10.0f, 0.0f });
	directionalLight.transform.SetLocalRotation({ 120.0f, -40.0f, 0.0f });
}

void AmberEngine::Core::SceneSystem::SceneManager::UnloadCurrentScene()
{
	if (m_currentScene)
	{
		delete m_currentScene;
		m_currentScene = nullptr;
		SceneUnloadEvent.Invoke();
	}
}

bool AmberEngine::Core::SceneSystem::SceneManager::HasCurrentScene() const
{
	return m_currentScene != nullptr;
}

AmberEngine::Core::SceneSystem::Scene* AmberEngine::Core::SceneSystem::SceneManager::GetCurrentScene()
{
	return m_currentScene;
}
