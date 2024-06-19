#pragma once

#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Core/EditorRenderer.h"
#include "AmberEngine/Core/PanelsManager.h"

#include "AmberEngine/Core/ECS/Actor.h"

#define EDITOR_EXEC(action)					AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>().action
#define EDITOR_BIND(method, ...)			std::bind(&AmberEngine::Core::EditorAction::method, &AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>(), ##__VA_ARGS__)
#define EDITOR_EVENT(target)				AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>().target
#define EDITOR_CONTEXT(instance)			AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>().GetContext().instance
#define EDITOR_RENDERER()					AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>().GetRenderer()
#define EDITOR_PANEL(type, id)				AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>().GetPanelsManager().GetPanelAs<type>(id)

namespace AmberEngine::Core
{
	class EditorAction
	{
	public:
		EditorAction(Context& p_context, EditorRenderer& p_editorRenderer, PanelsManager& p_panelsManager);
		~EditorAction() = default;

		Context& GetContext();
		EditorRenderer& GetRenderer();
		PanelsManager& GetPanelsManager();


		enum class EActorSpawnMode { ORIGIN, FRONT };

		void SetActorSpawnAtOrigin(bool p_value);

		void SetActorSpawnMode(EActorSpawnMode p_value);
		void ResetToDefaultLayout();
		void SaveLayout(const std::string& p_fileName);
		void SaveCurrentLayout();

		void SetLayout(const std::string& p_fileName);
		void SetSceneViewCameraSpeed(int p_value);
		int GetSceneViewCameraSpeed();

		void ResetSceneViewCameraPosition();

		glm::vec3 ComputeActorSpawnPoint(float p_distanceToCamera);

		ECS::Actor&	CreateEmptyActor(bool p_focusOnCreation = true, ECS::Actor* p_parent = nullptr, const std::string& p_name = "");
		ECS::Actor&	CreateActorWithModel(const std::string& p_path, bool p_focusOnCreation = true, ECS::Actor* p_parent = nullptr, const std::string& p_name = "");

		bool DestroyActor(ECS::Actor& p_actor);
		void DuplicateActor(ECS::Actor& p_toDuplicate, ECS::Actor* p_forcedParent = nullptr, bool p_focus = true);
		void SelectActor(ECS::Actor& p_target);
		void UnselectActor();
		bool IsAnyActorSelected() const;
		ECS::Actor& GetSelectedActor() const;

		void DelayAction(std::function<void()> p_action, uint32_t p_frames = 1);

		void ExecuteDelayedActions();

		std::string GetRealPath(const std::string& p_path);
		std::string GetResourcePath(const std::string& p_path, bool p_isFromEngine = false);

		void CompileShaders();

		void GenerateModelMaterialFiles(const std::string& materialName);

	private:
		std::string FindDuplicatedActorUniqueName(ECS::Actor& p_duplicated, ECS::Actor& p_newActor, SceneSystem::Scene& p_scene);

	public:
		Eventing::Event<ECS::Actor&> ActorSelectedEvent;
		Eventing::Event<ECS::Actor&> ActorUnselectedEvent;

		

	private:
		Context& m_context;
		PanelsManager& m_panelsManager;
		EditorRenderer& m_renderer;

		EActorSpawnMode m_actorSpawnMode = EActorSpawnMode::ORIGIN;

		std::vector<std::pair<uint32_t, std::function<void()>>> m_delayedActions;
	};
}
