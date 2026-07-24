#pragma once

#include "AmberEditor/Core/Context.h"
#include "AmberEditor/Core/EditorRenderer.h"
#include "AmberEditor/Core/PanelsManager.h"
#include "AmberCore/ECS/Actor.h"
#include "AmberTools/Utils/PathParser.h"

#include "AmberTools/Global/ServiceLocator.h"

#define EDITOR_EXEC(action) AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::EditorAction>().action
#define EDITOR_BIND(method, ...) std::bind(&AmberEditor::Core::EditorAction::method, &AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::EditorAction>(), ##__VA_ARGS__)
#define EDITOR_EVENT(target) AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::EditorAction>().target
#define EDITOR_CONTEXT(instance) AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::EditorAction>().GetContext().instance
#define EDITOR_RENDERER() AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::EditorAction>().GetRenderer()
#define EDITOR_PANEL(type, id) AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::EditorAction>().GetPanelsManager().GetPanelAs<type>(id)

namespace AmberEditor::Core
{
	class EditorAction
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

		std::string SelectBuildFolder();
		void Build(bool p_autoRun = false, bool p_tempFolder = false);
		void BuildAtLocation(const std::string& p_configuration, const std::string p_buildPath, bool p_autoRun = false);
		void SetActorSpawnMode(EActorSpawnMode p_value);
		void ResetToDefaultLayout();
		void SaveLayout(const std::string& p_fileName);
		void SaveCurrentLayout();
		void SetLayout(const std::string& p_fileName);

		void SetSceneViewCameraSpeed(int p_value);
		int GetSceneViewCameraSpeed();
		void ResetSceneViewCameraPosition();

		void SetAssetViewCameraSpeed(int p_value);
		int GetAssetViewCameraSpeed();
		void ResetAssetViewCameraPosition();

		void SetActorSpawnAtOrigin(bool p_value);
		glm::vec3 ComputeActorSpawnPoint(float p_distanceToCamera);

		AmberCore::ECS::Actor&	CreateEmptyActor(bool p_focusOnCreation = true, AmberCore::ECS::Actor* p_parent = nullptr, const std::string& p_name = "");
		AmberCore::ECS::Actor&	CreateActorWithModel(const std::string& p_path, bool p_focusOnCreation = true, AmberCore::ECS::Actor* p_parent = nullptr, const std::string& p_name = "");
		template<typename T> AmberCore::ECS::Actor& CreateMonoComponentActor(bool p_focusOnCreation = true, AmberCore::ECS::Actor* p_parent = nullptr);
		bool DestroyActor(AmberCore::ECS::Actor& p_actor);
		void DestroySelectedActors();
		void DuplicateActor(AmberCore::ECS::Actor& p_toDuplicate, AmberCore::ECS::Actor* p_forcedParent = nullptr, bool p_focus = true);
		void SelectActor(AmberCore::ECS::Actor& p_target);
		void AddActorToSelection(AmberCore::ECS::Actor& p_target);
		void RemoveActorFromSelection(AmberCore::ECS::Actor& p_target);
		void ToggleActorSelection(AmberCore::ECS::Actor& p_target);
		bool IsActorInSelection(AmberCore::ECS::Actor& p_target) const;
		void SelectActors(const std::vector<AmberCore::ECS::Actor*>& p_targets);
		void UnselectActor();
		void UnselectActors();
		bool IsAnyActorSelected() const;
		bool IsManyActorsSelected() const;
		AmberCore::ECS::Actor& GetSelectedActor() const;

		const std::vector<AmberCore::ECS::Actor*>& GetSelectedActors() const;

		void DelayAction(std::function<void()> p_action, uint32_t p_frames = 1);

		void ExecuteDelayedActions();

		std::string GetRealPath(const std::string& p_path);
		std::string GetResourcePath(const std::string& p_path, bool p_isFromEngine = false);
		std::string GetScriptPath(const std::string& p_path);

		void RefreshScripts();

		void CompileShaders();

		void SaveMaterials();

		void GenerateModelMaterialFiles(const std::string& p_materialPath, const std::string& p_shaderPath);

		bool ImportAsset(const std::string& p_initialDestinationDirectory);

		bool ImportAssetAtLocation(const std::string& p_destination);

		void PropagateFolderDestruction(std::string p_folderPath);

		void PropagateFileRename(std::string p_previousName, std::string p_newName);

		void PropagateScriptRename(std::string p_previousName, std::string p_newName);

		void PropagateFileRenameThroughSavedFilesOfType(const std::string& p_previousName, const std::string& p_newName, AmberTools::Utils::PathParser::EFileType p_fileType);

		void PropagateFolderRename(std::string p_previousName, std::string p_newName);


		enum class EEditorMode { EDIT, PLAY, PAUSE, FRAME_BY_FRAME };

		EEditorMode GetCurrentEditorMode() const;

		void SetEditorMode(EEditorMode p_newEditorMode);

		void StartPlaying();

		void PauseGame();

		void StopPlaying();

		void NextFrame();

		Context& GetContext();
		EditorRenderer& GetRenderer();
		PanelsManager& GetPanelsManager();

	private:
		std::string FindDuplicatedActorUniqueName(AmberCore::ECS::Actor& p_duplicated, AmberCore::ECS::Actor& p_newActor, AmberCore::SceneSystem::Scene& p_scene);

		void UpdateActorSelection();

	public:
		AmberTools::Eventing::Event<AmberCore::ECS::Actor&> ActorSelectedEvent;
		AmberTools::Eventing::Event<AmberCore::ECS::Actor&> ActorUnselectedEvent;
		AmberTools::Eventing::Event<> ActorSelectionEvent;
		AmberTools::Eventing::Event<EEditorMode> EditorModeChangedEvent;
		AmberTools::Eventing::Event<> PlayEvent;
		

	private:
		EEditorMode m_editorMode = EEditorMode::EDIT;
		Context& m_context;
		PanelsManager& m_panelsManager;
		EditorRenderer& m_renderer;
		tinyxml2::XMLDocument m_sceneBackup;
		EActorSpawnMode m_actorSpawnMode = EActorSpawnMode::ORIGIN;

		std::vector<std::pair<uint32_t, std::function<void()>>> m_delayedActions;
		std::vector<AmberCore::ECS::Actor*> m_selectedActors;
	};
}

#include "AmberEditor/Core/EditorAction.inl"