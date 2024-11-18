#include "Amberpch.h"

#include "AmberEngine/Core/SceneSystem/SceneManager.h"

#include "AmberEngine/Core/SceneSystem/Scene.h"

#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/ECS/Components/CAmbientSphereLight.h"
#include "AmberEngine/Core/ECS/Components/CDirectionalLight.h"

#include "AmberEngine/Windowing/Dialogs/MessageBox.h"

AmberEngine::Core::SceneSystem::SceneManager::SceneManager(const std::string& p_sceneRootFolder) : m_sceneRootFolder(p_sceneRootFolder)
{
	LoadEmptyScene();
}

AmberEngine::Core::SceneSystem::SceneManager::~SceneManager()
{
	UnloadCurrentScene();
}

void AmberEngine::Core::SceneSystem::SceneManager::Update()
{
	if (m_delayedLoadCall)
	{
		m_delayedLoadCall();
		m_delayedLoadCall = 0;
	}
}

void AmberEngine::Core::SceneSystem::SceneManager::LoadAndPlayDelayed(const std::string& p_path, bool p_isAbsolute)
{
	m_delayedLoadCall = [this, p_path, p_isAbsolute]
	{
		std::string previousSourcePath = GetCurrentSceneSourcePath();
		LoadScene(p_path, p_isAbsolute);
		StoreCurrentSceneSourcePath(previousSourcePath);
		GetCurrentScene()->Play();
	};
}

void AmberEngine::Core::SceneSystem::SceneManager::LoadEmptyScene()
{
	UnloadCurrentScene();

	m_currentScene = new Scene();

	SceneLoadEvent.Invoke();
}

void AmberEngine::Core::SceneSystem::SceneManager::LoadEmptyLightedScene()
{
	UnloadCurrentScene();

	m_currentScene = new Scene();

	SceneLoadEvent.Invoke();

	auto& directionalLight = m_currentScene->CreateActor("Directional Light");
	directionalLight.AddComponent<ECS::Components::CDirectionalLight>().SetIntensity(0.75f);
	directionalLight.transform.SetLocalPosition({ 0.0f, 10.0f, 0.0f });
	directionalLight.transform.SetLocalRotationEuler({ 120.0f, -40.0f, 0.0f });

	auto& ambientLight = m_currentScene->CreateActor("Ambient Light");
	ambientLight.AddComponent<ECS::Components::CAmbientSphereLight>().SetRadius(10000.0f);

	auto& camera = m_currentScene->CreateActor("Main Camera");
	camera.AddComponent<ECS::Components::CCamera>();
	camera.transform.SetLocalPosition({ 0.0f, 3.0f, 8.0f });
	camera.transform.SetLocalRotation(glm::quat(glm::radians(glm::vec3( 20.0f, 180.0f, 0.0f ))));
}

bool AmberEngine::Core::SceneSystem::SceneManager::LoadScene(const std::string& p_path, bool p_isAbsolute)
{
	std::string completePath = (p_isAbsolute ? "" : m_sceneRootFolder) + p_path;

	tinyxml2::XMLDocument xmlDocument;

	xmlDocument.LoadFile(completePath.c_str());

	if (LoadSceneFromMemory(xmlDocument))
	{
		StoreCurrentSceneSourcePath(completePath);

		SceneLoadEvent.Invoke();
		return true;
	}

	return false;
}

bool AmberEngine::Core::SceneSystem::SceneManager::LoadSceneFromMemory(tinyxml2::XMLDocument& p_xmlDocument)
{
	if (!p_xmlDocument.Error())
	{
		tinyxml2::XMLNode* xmlRootNode = p_xmlDocument.FirstChild();

		if (xmlRootNode)
		{
			tinyxml2::XMLNode* sceneNode = xmlRootNode->FirstChildElement("scene");

			if (sceneNode)
			{
				LoadEmptyScene();
				m_currentScene->OnDeserialize(p_xmlDocument, sceneNode);

				return true;
			}
		}
	}

	Windowing::Dialogs::MessageBox message("Scene loading failed", "The scene you are trying to load was not found or corrupted", Windowing::Dialogs::MessageBox::EMessageType::ERROR, Windowing::Dialogs::MessageBox::EButtonLayout::OK, true);

	return false;
}

void AmberEngine::Core::SceneSystem::SceneManager::UnloadCurrentScene()
{
	if (m_currentScene)
	{
		delete m_currentScene;
		m_currentScene = nullptr;
		SceneUnloadEvent.Invoke();
	}

	ForgetCurrentSceneSourcePath();
}

bool AmberEngine::Core::SceneSystem::SceneManager::HasCurrentScene() const
{
	return m_currentScene != nullptr;
}

AmberEngine::Core::SceneSystem::Scene* AmberEngine::Core::SceneSystem::SceneManager::GetCurrentScene() const
{
	return m_currentScene;
}

std::string AmberEngine::Core::SceneSystem::SceneManager::GetCurrentSceneSourcePath() const
{
	return m_currentSceneSourcePath;
}

bool AmberEngine::Core::SceneSystem::SceneManager::IsCurrentSceneLoadedFromDisk() const
{
	return m_currentSceneLoadedFromPath;
}

void AmberEngine::Core::SceneSystem::SceneManager::StoreCurrentSceneSourcePath(const std::string& p_path)
{
	m_currentSceneSourcePath = p_path;
	m_currentSceneLoadedFromPath = true;
	CurrentSceneSourcePathChangedEvent.Invoke(m_currentSceneSourcePath);
}

void AmberEngine::Core::SceneSystem::SceneManager::ForgetCurrentSceneSourcePath()
{
	m_currentSceneSourcePath = "";
	m_currentSceneLoadedFromPath = false;
	CurrentSceneSourcePathChangedEvent.Invoke(m_currentSceneSourcePath);
}
