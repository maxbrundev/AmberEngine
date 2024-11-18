#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Eventing/Event.h"

#include "AmberEngine/Tools/FileSystem/tinyxml2.h"

namespace AmberEngine::Core::SceneSystem
{
	class Scene;

	class API_AMBERENGINE SceneManager
	{
	public:
		SceneManager(const std::string& p_sceneRootFolder = "");
		~SceneManager();

		void Update();

		void LoadAndPlayDelayed(const std::string& p_path, bool p_isAbsolute = false);
		void LoadEmptyScene();
		void LoadEmptyLightedScene();
		bool LoadScene(const std::string& p_path, bool p_isAbsolute = false);
		bool LoadSceneFromMemory(tinyxml2::XMLDocument& p_xmlDocument);

		void UnloadCurrentScene();

		bool HasCurrentScene() const;
		Scene* GetCurrentScene() const;

		std::string GetCurrentSceneSourcePath() const;
		bool IsCurrentSceneLoadedFromDisk() const;
		void StoreCurrentSceneSourcePath(const std::string& p_path);
		void ForgetCurrentSceneSourcePath();

	public:
		Eventing::Event<> SceneLoadEvent;
		Eventing::Event<> SceneUnloadEvent;
		Eventing::Event<const std::string&> CurrentSceneSourcePathChangedEvent;

	private:
		const std::string m_sceneRootFolder;
		std::string m_currentSceneSourcePath = "";

		Scene* m_currentScene = nullptr;

		std::function<void()> m_delayedLoadCall;

		bool m_currentSceneLoadedFromPath = false;
	};
}
