#pragma once

#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Core/EditorRenderer.h"
#include "AmberEngine/Core/PanelsManager.h"
#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Tools/Utils/PathParser.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"

#define EDITOR_EXEC(action) AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>().action
#define EDITOR_BIND(method, ...) std::bind(&AmberEngine::Core::EditorAction::method, &AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>(), ##__VA_ARGS__)
#define EDITOR_EVENT(target) AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>().target
#define EDITOR_CONTEXT(instance) AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>().GetContext().instance
#define EDITOR_RENDERER() AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>().GetRenderer()
#define EDITOR_PANEL(type, id) AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::EditorAction>().GetPanelsManager().GetPanelAs<type>(id)

namespace AmberEngine::Core
{
	class API_AMBERENGINE EditorAction
	{
	public:

		enum class EActorSpawnMode
		{
			ORIGIN,
			FRONT
		};

		EditorAction(Context& p_context, EditorRenderer& p_editorRenderer, PanelsManager& p_panelsManager);
		~EditorAction() = default;

		void LoadEmptyScene();
		void SaveCurrentSceneTo(const std::string& p_path);
		void LoadSceneFromDisk(const std::string& p_path, bool p_isAbsolute = false);
		bool IsCurrentSceneLoadedFromDisk() const;
		void SaveSceneChanges();
		void SaveAs();
		void SetActorSpawnMode(EActorSpawnMode p_value);
		void ResetToDefaultLayout();
		void SaveLayout(const std::string& p_fileName);
		void SaveCurrentLayout();
		void SetLayout(const std::string& p_fileName);

		void SetSceneViewCameraSpeed(int p_value);
		int GetSceneViewCameraSpeed();
		void ResetSceneViewCameraPosition();

		void SetActorSpawnAtOrigin(bool p_value);
		glm::vec3 ComputeActorSpawnPoint(float p_distanceToCamera);

		ECS::Actor&	CreateEmptyActor(bool p_focusOnCreation = true, ECS::Actor* p_parent = nullptr, const std::string& p_name = "");
		ECS::Actor&	CreateActorWithModel(const std::string& p_path, bool p_focusOnCreation = true, ECS::Actor* p_parent = nullptr, const std::string& p_name = "");
		template<typename T> ECS::Actor& CreateMonoComponentActor(bool p_focusOnCreation = true, ECS::Actor* p_parent = nullptr);
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

		bool ImportAsset(const std::string& p_initialDestinationDirectory);

		bool ImportAssetAtLocation(const std::string& p_destination);

		void PropagateFolderDestruction(std::string p_folderPath);

		void PropagateFileRename(std::string p_previousName, std::string p_newName);

		void PropagateFileRenameThroughSavedFilesOfType(const std::string& p_previousName, const std::string& p_newName, Tools::Utils::PathParser::EFileType p_fileType);

		void PropagateFolderRename(std::string p_previousName, std::string p_newName);


		enum class EEditorMode { EDIT, PLAY, PAUSE, FRAME_BY_FRAME };

		/**
		* Returns the current editor state/mode
		*/
		EEditorMode GetCurrentEditorMode() const;

		/**
		* Defines the editor state/mode
		* @param p_newEditorMode
		*/
		void SetEditorMode(EEditorMode p_newEditorMode);

		/**
		* Start playing the current scene and update the editor mode
		*/
		void StartPlaying();

		/**
		* Pause the current playing scene and update the editor mode
		*/
		void PauseGame();

		/**
		* Stop platying the current scene and update the editor mode
		*/
		void StopPlaying();

		/**
		* Play the current frame and pause the editor
		*/
		void NextFrame();

		Context& GetContext();
		EditorRenderer& GetRenderer();
		PanelsManager& GetPanelsManager();

	private:
		std::string FindDuplicatedActorUniqueName(ECS::Actor& p_duplicated, ECS::Actor& p_newActor, SceneSystem::Scene& p_scene);

	public:
		Eventing::Event<ECS::Actor&> ActorSelectedEvent;
		Eventing::Event<ECS::Actor&> ActorUnselectedEvent;
		Eventing::Event<EEditorMode> EditorModeChangedEvent;
		Eventing::Event<> PlayEvent;
		

	private:
		EEditorMode m_editorMode = EEditorMode::EDIT;
		Context& m_context;
		PanelsManager& m_panelsManager;
		EditorRenderer& m_renderer;
		tinyxml2::XMLDocument m_sceneBackup;
		EActorSpawnMode m_actorSpawnMode = EActorSpawnMode::ORIGIN;

		std::vector<std::pair<uint32_t, std::function<void()>>> m_delayedActions;
	};
}

#include "AmberEngine/Core/EditorAction.inl"