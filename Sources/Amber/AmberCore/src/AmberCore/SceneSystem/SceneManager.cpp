#include "Amberpch.h"

#include "AmberCore/SceneSystem/SceneManager.h"

#include "AmberCore/SceneSystem/Scene.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberCore/ECS/Components/CAmbientSphereLight.h"
#include "AmberCore/ECS/Components/CDirectionalLight.h"

#include "AmberWindowing/Dialogs/MessageBox.h"

AmberCore::SceneSystem::SceneManager::SceneManager(const std::string& p_sceneRootFolder) : m_sceneRootFolder(p_sceneRootFolder)
{
	LoadEmptyScene();
}

AmberCore::SceneSystem::SceneManager::~SceneManager()
{
	UnloadCurrentScene();
}

void AmberCore::SceneSystem::SceneManager::Update()
{
	if (m_delayedLoadCall)
	{
		m_delayedLoadCall();
		m_delayedLoadCall = 0;
	}
}

void AmberCore::SceneSystem::SceneManager::LoadAndPlayDelayed(const std::string& p_path, bool p_isAbsolute)
{
	m_delayedLoadCall = [this, p_path, p_isAbsolute]
	{
		std::string previousSourcePath = GetCurrentSceneSourcePath();
		LoadScene(p_path, p_isAbsolute);
		StoreCurrentSceneSourcePath(previousSourcePath);
		GetCurrentScene()->Play();
	};
}

void AmberCore::SceneSystem::SceneManager::LoadEmptyScene()
{
	UnloadCurrentScene();

	m_currentScene = new Scene();

	SceneLoadEvent.Invoke();
}

void AmberCore::SceneSystem::SceneManager::LoadEmptyLightedScene()
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

bool AmberCore::SceneSystem::SceneManager::LoadScene(const std::string& p_path, bool p_isAbsolute)
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

bool AmberCore::SceneSystem::SceneManager::LoadSceneFromMemory(tinyxml2::XMLDocument& p_xmlDocument)
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

	AmberWindowing::Dialogs::MessageBox message("Scene loading failed", "The scene you are trying to load was not found or corrupted", AmberWindowing::Dialogs::MessageBox::EMessageType::ERROR, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK, true);

	return false;
}

void AmberCore::SceneSystem::SceneManager::UnloadCurrentScene()
{
	if (m_currentScene)
	{
		delete m_currentScene;
		m_currentScene = nullptr;
		SceneUnloadEvent.Invoke();
	}

	ForgetCurrentSceneSourcePath();
}

bool AmberCore::SceneSystem::SceneManager::HasCurrentScene() const
{
	return m_currentScene != nullptr;
}

AmberCore::SceneSystem::Scene* AmberCore::SceneSystem::SceneManager::GetCurrentScene() const
{
	return m_currentScene;
}

std::string AmberCore::SceneSystem::SceneManager::GetCurrentSceneSourcePath() const
{
	return m_currentSceneSourcePath;
}

bool AmberCore::SceneSystem::SceneManager::IsCurrentSceneLoadedFromDisk() const
{
	return m_currentSceneLoadedFromPath;
}

void AmberCore::SceneSystem::SceneManager::StoreCurrentSceneSourcePath(const std::string& p_path)
{
	m_currentSceneSourcePath = p_path;
	m_currentSceneLoadedFromPath = true;
	CurrentSceneSourcePathChangedEvent.Invoke(m_currentSceneSourcePath);
}

void AmberCore::SceneSystem::SceneManager::ForgetCurrentSceneSourcePath()
{
	m_currentSceneSourcePath = "";
	m_currentSceneLoadedFromPath = false;
	CurrentSceneSourcePathChangedEvent.Invoke(m_currentSceneSourcePath);
}
