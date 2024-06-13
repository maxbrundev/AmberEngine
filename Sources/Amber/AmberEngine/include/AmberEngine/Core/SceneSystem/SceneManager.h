#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Eventing/Event.h"

namespace AmberEngine::Core::SceneSystem
{
	class Scene;

	class API_AMBERENGINE SceneManager
	{
	public:
		SceneManager(const std::string& p_sceneRootFolder = "");
		~SceneManager();

		void LoadEmptyScene();
		void LoadEmptyLightedScene();
		void UnloadCurrentScene();
		bool HasCurrentScene() const;
		Scene* GetCurrentScene();

	public:
		Eventing::Event<> SceneLoadEvent;
		Eventing::Event<> SceneUnloadEvent;

	private:
		Scene* m_currentScene = nullptr;
	};
}
