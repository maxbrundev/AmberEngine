#include "Amberpch.h"

#include "AmberEngine/Core/EditorAction.h"

#include <variant>

#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"
#include "AmberEngine/Core/ECS/Components/CMaterialRenderer.h"
#include "AmberEngine/Resources/Loaders/ShaderLoader.h"
#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/UI/Panels/Views/SceneView.h"
#include "AmberEngine/Core/SceneSystem/Scene.h"
#include "AmberEngine/Tools/Utils/PathParser.h"
#include "AmberEngine/Tools/Utils/String.h"
#include "AmberEngine/UI/Panels/Inspector.h"
#include "AmberEngine/UI/Panels/MaterialEditor.h"
#include "AmberEngine/UI/Panels/Views/AssetView.h"
#include "AmberEngine/UI/Panels/Views/GameView.h"
#include "AmberEngine/Windowing/Dialogs/OpenFileDialog.h"
#include "AmberEngine/Windowing/Dialogs/SaveFileDialog.h"
#include "AmberEngine/Windowing/Dialogs/MessageBox.h"

AmberEngine::Core::EditorAction::EditorAction(Context& p_context, EditorRenderer& p_editorRenderer, PanelsManager& p_panelsManager) :
	m_context(p_context),
	m_renderer(p_editorRenderer),
	m_panelsManager(p_panelsManager)
{
	Tools::Global::ServiceLocator::Provide<EditorAction>(*this);
}

AmberEngine::Core::Context& AmberEngine::Core::EditorAction::GetContext()
{
	return m_context;
}

AmberEngine::Core::EditorRenderer& AmberEngine::Core::EditorAction::GetRenderer()
{
	return m_renderer;
}

AmberEngine::Core::PanelsManager& AmberEngine::Core::EditorAction::GetPanelsManager()
{
	return m_panelsManager;
}

void AmberEngine::Core::EditorAction::SetActorSpawnAtOrigin(bool p_value)
{
	if (p_value)
		m_actorSpawnMode = EActorSpawnMode::ORIGIN;
	else
		m_actorSpawnMode = EActorSpawnMode::FRONT;
}

void AmberEngine::Core::EditorAction::LoadEmptyScene()
{
	m_context.sceneManager.LoadEmptyLightedScene();
}

void AmberEngine::Core::EditorAction::SaveCurrentSceneTo(const std::string& p_path)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* node = doc.NewElement("root");
	doc.InsertFirstChild(node);
	m_context.sceneManager.StoreCurrentSceneSourcePath(p_path);
	m_context.sceneManager.GetCurrentScene()->OnSerialize(doc, node);
	doc.SaveFile(p_path.c_str());
}

void AmberEngine::Core::EditorAction::LoadSceneFromDisk(const std::string& p_path, bool p_isAbsolute)
{
	m_context.sceneManager.LoadScene(p_path, p_isAbsolute);
	m_panelsManager.GetPanelAs<UI::Panels::SceneView>("Scene View").Focus();
}

bool AmberEngine::Core::EditorAction::IsCurrentSceneLoadedFromDisk() const
{
	return m_context.sceneManager.IsCurrentSceneLoadedFromDisk();
}

void AmberEngine::Core::EditorAction::SaveSceneChanges()
{
	if (IsCurrentSceneLoadedFromDisk())
	{
		SaveCurrentSceneTo(m_context.sceneManager.GetCurrentSceneSourcePath());
	}
	else
	{
		SaveAs();
	}
}

void AmberEngine::Core::EditorAction::SaveAs()
{
	Windowing::Dialogs::SaveFileDialog dialog("New Scene");
	dialog.SetInitialDirectory(m_context.projectAssetsPath + "New Scene");
	dialog.DefineExtension("Amber Scene", ".abscene");
	dialog.Show();

	if (dialog.HasSucceeded())
	{
		if (dialog.IsFileExisting())
		{
			Windowing::Dialogs::MessageBox message("File already exists!", "The file \"" + dialog.GetSelectedFileName() + "\" already exists.\n\nUsing this file as the new home for your scene will erase any content stored in this file.\n\nAre you ok with that?", Windowing::Dialogs::MessageBox::EMessageType::WARNING, Windowing::Dialogs::MessageBox::EButtonLayout::YES_NO, true);
			switch (message.GetUserAction())
			{
			case Windowing::Dialogs::MessageBox::EUserAction::YES: break;
			case Windowing::Dialogs::MessageBox::EUserAction::NO: return;
			}
		}

		SaveCurrentSceneTo(dialog.GetSelectedFilePath());
	}
}

void AmberEngine::Core::EditorAction::SetActorSpawnMode(EActorSpawnMode p_value)
{
	m_actorSpawnMode = p_value;
}

void AmberEngine::Core::EditorAction::ResetToDefaultLayout()
{
	DelayAction([this]() {m_context.uiManager->LoadLayout("Config\\layout.ini"); });
}

void AmberEngine::Core::EditorAction::SaveLayout(const std::string& p_fileName)
{
	DelayAction([&]() {m_context.uiManager->SaveLayout(std::ref(p_fileName)); });
}

void AmberEngine::Core::EditorAction::SaveCurrentLayout()
{
	DelayAction([&]() {m_context.uiManager->SaveCurrentLayout(); });
}

void AmberEngine::Core::EditorAction::SetLayout(const std::string& p_fileName)
{
	DelayAction([&]() {m_context.uiManager->SetLayout(std::ref(p_fileName)); });
}

void AmberEngine::Core::EditorAction::SetSceneViewCameraSpeed(int p_value)
{
	EDITOR_PANEL(UI::Panels::SceneView, "Scene View").GetCameraController().SetSpeed((float)p_value);
}

int AmberEngine::Core::EditorAction::GetSceneViewCameraSpeed()
{
	return (int)EDITOR_PANEL(UI::Panels::SceneView, "Scene View").GetCameraController().GetSpeed();
}

void AmberEngine::Core::EditorAction::ResetSceneViewCameraPosition()
{
	EDITOR_PANEL(UI::Panels::SceneView, "Scene View").GetCameraController().SetPosition({ -10.0f, 4.0f, 10.0f });
}

glm::vec3 AmberEngine::Core::EditorAction::ComputeActorSpawnPoint(float p_distanceToCamera)
{
	auto& sceneView = m_panelsManager.GetPanelAs<UI::Panels::SceneView>("Scene View");
	return sceneView.GetCameraPosition() + sceneView.GetCameraRotation() * glm::vec3(0.0f, 0.0f, 1.0f) * p_distanceToCamera;
}

AmberEngine::Core::ECS::Actor& AmberEngine::Core::EditorAction::CreateEmptyActor(bool p_focusOnCreation, ECS::Actor* p_parent, const std::string& p_name)
{
	const auto currentScene = m_context.sceneManager.GetCurrentScene();
	auto& instance = p_name.empty() ? currentScene->CreateActor() : currentScene->CreateActor(p_name);

	if (p_parent)
		instance.SetParent(*p_parent);

	if (m_actorSpawnMode == EActorSpawnMode::FRONT)
		instance.transform.SetLocalPosition(ComputeActorSpawnPoint(10.0f));

	if (p_focusOnCreation)
		SelectActor(instance);

	return instance;
}

AmberEngine::Core::ECS::Actor& AmberEngine::Core::EditorAction::CreateActorWithModel(const std::string& p_path, bool p_focusOnCreation, ECS::Actor* p_parent, const std::string& p_name)
{
	auto& instance = CreateEmptyActor(false, p_parent, p_name);

	auto& modelRenderer = instance.AddComponent<ECS::Components::CModelRenderer>();

	const auto model = m_context.modelManager[p_path];
	if (model)
		modelRenderer.SetModel(model);

	auto& materialRenderer = instance.AddComponent<ECS::Components::CMaterialRenderer>();
	const auto material = m_context.materialManager[":Materials\\Default.abmat"];
	if (material)
		materialRenderer.FillWithMaterial(*material);

	if (p_focusOnCreation)
		SelectActor(instance);

	return instance;
}

bool AmberEngine::Core::EditorAction::DestroyActor(ECS::Actor& p_actor)
{
	p_actor.MarkAsDestroy();

	return true;
}

void AmberEngine::Core::EditorAction::DuplicateActor(ECS::Actor& p_toDuplicate, ECS::Actor* p_forcedParent, bool p_focus)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* actorsRoot = doc.NewElement("actors");
	p_toDuplicate.OnSerialize(doc, actorsRoot);
	auto& newActor = CreateEmptyActor(false);
	int64_t idToUse = newActor.GetID();
	tinyxml2::XMLElement* currentActor = actorsRoot->FirstChildElement("actor");
	newActor.OnDeserialize(doc, currentActor);
	
	newActor.SetID(idToUse);
	
	if (p_forcedParent)
		newActor.SetParent(*p_forcedParent);
	else
	{
		auto currentScene = m_context.sceneManager.GetCurrentScene();
	
		if (newActor.GetParentID() > 0)
		{
			if (auto found = currentScene->FindActorByID(newActor.GetParentID()); found)
			{
				newActor.SetParent(*found);
			}
		}
	
		const auto uniqueName = FindDuplicatedActorUniqueName(p_toDuplicate, newActor, *currentScene);
		newActor.SetName(uniqueName);
	}
	
	if (p_focus)
		SelectActor(newActor);
	
	for (auto& child : p_toDuplicate.GetChildren())
		DuplicateActor(*child, &newActor, false);
}

void AmberEngine::Core::EditorAction::SelectActor(ECS::Actor& p_target)
{
	EDITOR_PANEL(UI::Panels::Inspector, "Inspector").FocusActor(p_target);
}

void AmberEngine::Core::EditorAction::UnselectActor()
{
	EDITOR_PANEL(UI::Panels::Inspector, "Inspector").UnFocus();
}

bool AmberEngine::Core::EditorAction::IsAnyActorSelected() const
{
	return EDITOR_PANEL(UI::Panels::Inspector, "Inspector").GetTargetActor();
}

AmberEngine::Core::ECS::Actor& AmberEngine::Core::EditorAction::GetSelectedActor() const
{
	return *EDITOR_PANEL(UI::Panels::Inspector, "Inspector").GetTargetActor();
}

void AmberEngine::Core::EditorAction::DelayAction(std::function<void()> p_action, uint32_t p_frames)
{
	m_delayedActions.emplace_back(p_frames + 1, p_action);
}

void AmberEngine::Core::EditorAction::ExecuteDelayedActions()
{
	std::for_each(m_delayedActions.begin(), m_delayedActions.end(), [](std::pair<uint32_t, std::function<void()>> & p_element)
	{
		--p_element.first;

		if (p_element.first == 0)
			p_element.second();
	});

	m_delayedActions.erase(std::remove_if(m_delayedActions.begin(), m_delayedActions.end(), [](std::pair<uint32_t, std::function<void()>> & p_element)
	{
		return p_element.first == 0;
	}), m_delayedActions.end());
}

std::string AmberEngine::Core::EditorAction::GetRealPath(const std::string& p_path)
{
	std::string result;

	if (p_path[0] == ':') // The path is an engine path
	{
		result = m_context.engineAssetsPath + std::string(p_path.data() + 1, p_path.data() + p_path.size());
	}
	else // The path is a project path
	{
		result = m_context.projectAssetsPath + p_path;
	}

	return result;
}

std::string AmberEngine::Core::EditorAction::GetResourcePath(const std::string& p_path, bool p_isFromEngine)
{
	std::string result = p_path;

	if (Tools::Utils::String::Replace(result, p_isFromEngine ? m_context.engineAssetsPath : m_context.projectAssetsPath, ""))
	{
		if (p_isFromEngine)
			result = ':' + result;
	}

	return result;
}

void AmberEngine::Core::EditorAction::CompileShaders()
{
	for (auto shader : m_context.shaderManager.GetResources())
		Resources::Loaders::ShaderLoader::Recompile(*shader.second, GetRealPath(shader.second->Path));
}

void AmberEngine::Core::EditorAction::GenerateModelMaterialFiles(const std::string& materialName)
{
	std::string finalPath = m_context.projectAssetsPath + "Materials\\" + materialName + ".abmat";

	if (std::filesystem::exists(finalPath))
		return;


	{
		if (!std::filesystem::exists(m_context.projectAssetsPath + "Materials\\"))
		{
			std::filesystem::create_directories(m_context.projectAssetsPath + "Materials\\");
		}

		std::ofstream outfile(finalPath);
		outfile << "<root><shader>:Shaders\\Standard.glsl</shader></root>" << std::endl; // Empty standard material content
	}

	

	EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))];
}

bool AmberEngine::Core::EditorAction::ImportAsset(const std::string& p_initialDestinationDirectory)
{
	std::string modelFormats = "*.fbx;*.obj;";
	std::string textureFormats = "*.png;*.jpeg;*.jpg;*.tga";
	std::string shaderFormats = "*.glsl;";

	Windowing::Dialogs::OpenFileDialog selectAssetDialog("Select an asset to import");
	selectAssetDialog.AddFileType("Any supported format", modelFormats + textureFormats + shaderFormats);
	selectAssetDialog.AddFileType("Model (.fbx, .obj)", modelFormats);
	selectAssetDialog.AddFileType("Texture (.png, .jpeg, .jpg, .tga)", textureFormats);
	selectAssetDialog.AddFileType("Shader (.glsl)", shaderFormats);
	selectAssetDialog.Show();

	if(selectAssetDialog.HasSucceeded())
	{
		std::string source = selectAssetDialog.GetSelectedFilePath();
		std::string extension = '.' + Tools::Utils::PathParser::GetExtension(source);
		std::string filename = selectAssetDialog.GetSelectedFileName();

		Windowing::Dialogs::SaveFileDialog saveLocationDialog("Where to import?");
		saveLocationDialog.SetInitialDirectory(p_initialDestinationDirectory + filename);
		saveLocationDialog.DefineExtension(extension, extension);
		saveLocationDialog.Show();

		if (saveLocationDialog.HasSucceeded())
		{
			std::string destination = saveLocationDialog.GetSelectedFilePath();

			if (!std::filesystem::exists(destination) || Windowing::Dialogs::MessageBox("File already exists", "The destination you have selected already exists, importing this file will erase the previous file content, are you sure about that?", Windowing::Dialogs::MessageBox::EMessageType::WARNING, Windowing::Dialogs::MessageBox::EButtonLayout::OK_CANCEL).GetUserAction() == Windowing::Dialogs::MessageBox::EUserAction::OK)
			{
				std::filesystem::copy(source, destination, std::filesystem::copy_options::overwrite_existing);
				
				return true;
			}
		}
	}

	return false;
}

bool AmberEngine::Core::EditorAction::ImportAssetAtLocation(const std::string& p_destination)
{
	std::string modelFormats = "*.fbx;*.obj;";
	std::string textureFormats = "*.png;*.jpeg;*.jpg;*.tga;";
	std::string shaderFormats = "*.glsl;";

	Windowing::Dialogs::OpenFileDialog selectAssetDialog("Select an asset to import");
	selectAssetDialog.AddFileType("Any supported format", modelFormats + textureFormats + shaderFormats);
	selectAssetDialog.AddFileType("Model (.fbx, .obj)", modelFormats);
	selectAssetDialog.AddFileType("Texture (.png, .jpeg, .jpg, .tga)", textureFormats);
	selectAssetDialog.AddFileType("Shader (.glsl)", shaderFormats);
	selectAssetDialog.Show();

	if (selectAssetDialog.HasSucceeded())
	{
		std::string source = selectAssetDialog.GetSelectedFilePath();
		std::string destination = p_destination + selectAssetDialog.GetSelectedFileName();

		if (!std::filesystem::exists(destination) || Windowing::Dialogs::MessageBox("File already exists", "The destination you have selected already exists, importing this file will erase the previous file content, are you sure about that?", Windowing::Dialogs::MessageBox::EMessageType::WARNING, Windowing::Dialogs::MessageBox::EButtonLayout::OK_CANCEL).GetUserAction() == Windowing::Dialogs::MessageBox::EUserAction::OK)
		{
			std::filesystem::copy(source, destination, std::filesystem::copy_options::overwrite_existing);
			return true;
		}
	}

	return false;
}

void AmberEngine::Core::EditorAction::PropagateFolderDestruction(std::string p_folderPath)
{
	for (auto& p : std::filesystem::recursive_directory_iterator(p_folderPath))
	{
		if (!p.is_directory())
		{
			PropagateFileRename(Tools::Utils::PathParser::MakeWindowsStyle(p.path().string()), "?");
		}
	}
}

void AmberEngine::Core::EditorAction::PropagateFileRename(std::string p_previousName, std::string p_newName)
{
	p_previousName = GetResourcePath(p_previousName);
	p_newName = GetResourcePath(p_newName);

	if (p_newName != "?")
	{
		/* If not a real rename is asked (Not delete) */

		if (Tools::Global::ServiceLocator::Get<ResourceManagement::ModelManager>().MoveResource(p_previousName, p_newName))
		{
			Resources::Model* resource = Tools::Global::ServiceLocator::Get<ResourceManagement::ModelManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(Resources::Model, Path)) = p_newName;
		}

		if (Tools::Global::ServiceLocator::Get<ResourceManagement::TextureManager>().MoveResource(p_previousName, p_newName))
		{
			Resources::Texture* resource = Tools::Global::ServiceLocator::Get<ResourceManagement::TextureManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(Resources::Texture, Path)) = p_newName;
		}

		if (Tools::Global::ServiceLocator::Get<ResourceManagement::ShaderManager>().MoveResource(p_previousName, p_newName))
		{
			Resources::Shader* resource = Tools::Global::ServiceLocator::Get<ResourceManagement::ShaderManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(Resources::Shader, Path)) = p_newName;
		}

		if (Tools::Global::ServiceLocator::Get<ResourceManagement::MaterialManager>().MoveResource(p_previousName, p_newName))
		{
			Resources::Material* resource = Tools::Global::ServiceLocator::Get<ResourceManagement::MaterialManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(Resources::Material, path)) = p_newName;
		}
	}
	else
	{
		if (auto texture = Tools::Global::ServiceLocator::Get<ResourceManagement::TextureManager>().GetResource(p_previousName, false))
		{
			for (auto[name, instance] : Tools::Global::ServiceLocator::Get<ResourceManagement::MaterialManager>().GetResources())
				if (instance)
					for (auto&[name, value] : instance->GetUniformsData())
						if (value.has_value() && value.type() == typeid(Resources::Texture*))
							if (std::any_cast<Resources::Texture*>(value) == texture)
								value = static_cast<Resources::Texture*>(nullptr);

			auto& assetView = EDITOR_PANEL(UI::Panels::AssetView, "Asset View");
			auto assetViewRes = assetView.GetResource();
			if (auto pval = std::get_if<Resources::Texture*>(&assetViewRes); pval && *pval)
				assetView.SetResource(static_cast<Resources::Texture*>(nullptr));

			Tools::Global::ServiceLocator::Get<ResourceManagement::TextureManager>().UnloadResource(p_previousName);
		}

		if (auto shader = Tools::Global::ServiceLocator::Get<ResourceManagement::ShaderManager>().GetResource(p_previousName, false))
		{
			auto& materialEditor = EDITOR_PANEL(UI::Panels::MaterialEditor, "Material Editor");

			for (auto[name, instance] : Tools::Global::ServiceLocator::Get<ResourceManagement::MaterialManager>().GetResources())
				if (instance && instance->GetShader() == shader)
					instance->SetShader(nullptr);

			Tools::Global::ServiceLocator::Get<ResourceManagement::ShaderManager>().UnloadResource(p_previousName);
		}

		if (auto model = Tools::Global::ServiceLocator::Get<ResourceManagement::ModelManager>().GetResource(p_previousName, false))
		{
			auto& assetView = EDITOR_PANEL(UI::Panels::AssetView, "Asset View");
			auto assetViewRes = assetView.GetResource();
			if (auto pval = std::get_if<Resources::Model*>(&assetViewRes); pval && *pval)
				assetView.SetResource(static_cast<Resources::Model*>(nullptr));

			if (auto currentScene = m_context.sceneManager.GetCurrentScene())
				for (auto actor : currentScene->GetActors())
					if (auto modelRenderer = actor->GetComponent<ECS::Components::CModelRenderer>(); modelRenderer && modelRenderer->GetModel() == model)
						modelRenderer->SetModel(nullptr);

			Tools::Global::ServiceLocator::Get<ResourceManagement::ModelManager>().UnloadResource(p_previousName);
		}

		if (auto material = Tools::Global::ServiceLocator::Get<ResourceManagement::MaterialManager>().GetResource(p_previousName, false))
		{
			auto& materialEditor = EDITOR_PANEL(UI::Panels::MaterialEditor, "Material Editor");

			if (materialEditor.GetTarget() == material)
				materialEditor.RemoveTarget();

			auto& assetView = EDITOR_PANEL(UI::Panels::AssetView, "Asset View");
			auto assetViewRes = assetView.GetResource();
			if (auto pval = std::get_if<Resources::Material*>(&assetViewRes); pval && *pval)
				assetView.SetResource(static_cast<Resources::Material*>(nullptr));

			if (auto currentScene = m_context.sceneManager.GetCurrentScene())
				for (auto actor : currentScene->GetActors())
					if (auto materialRenderer = actor->GetComponent<ECS::Components::CMaterialRenderer>(); materialRenderer)
						materialRenderer->RemoveMaterialByInstance(*material);

			Tools::Global::ServiceLocator::Get<ResourceManagement::MaterialManager>().UnloadResource(p_previousName);
		}
	}

	switch (Tools::Utils::PathParser::GetFileType(p_previousName))
	{
	case Tools::Utils::PathParser::EFileType::MATERIAL:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, Tools::Utils::PathParser::EFileType::SCENE);
		break;
	case Tools::Utils::PathParser::EFileType::MODEL:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, Tools::Utils::PathParser::EFileType::SCENE);
		break;
	case Tools::Utils::PathParser::EFileType::SHADER:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, Tools::Utils::PathParser::EFileType::MATERIAL);
		break;
	case Tools::Utils::PathParser::EFileType::TEXTURE:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, Tools::Utils::PathParser::EFileType::MATERIAL);
		break;
	}

	EDITOR_PANEL(UI::Panels::Inspector, "Inspector").Refresh();
	EDITOR_PANEL(UI::Panels::MaterialEditor, "Material Editor").Refresh();
}

void AmberEngine::Core::EditorAction::PropagateFileRenameThroughSavedFilesOfType(const std::string& p_previousName,
	const std::string& p_newName, Tools::Utils::PathParser::EFileType p_fileType)
{
	for (auto& entry : std::filesystem::recursive_directory_iterator(m_context.projectAssetsPath))
	{
		if (Tools::Utils::PathParser::GetFileType(entry.path().string()) == p_fileType)
		{
			using namespace std;

			{
				ifstream in(entry.path().string().c_str());
				ofstream out("TEMP");
				string wordToReplace(">" + p_previousName + "<");
				string wordToReplaceWith(">" + p_newName + "<");

				string line;
				size_t len = wordToReplace.length();
				while (getline(in, line))
				{
					Tools::Utils::String::Replace(line, wordToReplace, wordToReplaceWith);
				}

				out.close(); in.close();
			}

			std::filesystem::copy_file("TEMP", entry.path(), std::filesystem::copy_options::overwrite_existing);
			std::filesystem::remove("TEMP");
		}
	}
}

void AmberEngine::Core::EditorAction::PropagateFolderRename(std::string p_previousName, std::string p_newName)
{
	p_previousName = Tools::Utils::PathParser::MakeNonWindowsStyle(p_previousName);
	p_newName = Tools::Utils::PathParser::MakeNonWindowsStyle(p_newName);

	for (auto& p : std::filesystem::recursive_directory_iterator(p_newName))
	{
		if (!p.is_directory())
		{
			std::string newFileName = Tools::Utils::PathParser::MakeNonWindowsStyle(p.path().string());
			std::string previousFileName;

			for (char c : newFileName)
			{
				previousFileName += c;
				if (previousFileName == p_newName)
					previousFileName = p_previousName;
			}

			PropagateFileRename(Tools::Utils::PathParser::MakeWindowsStyle(previousFileName), Tools::Utils::PathParser::MakeWindowsStyle(newFileName));
		}
	}
}

AmberEngine::Core::EditorAction::EEditorMode AmberEngine::Core::EditorAction::GetCurrentEditorMode() const
{
	return m_editorMode;
}

void AmberEngine::Core::EditorAction::SetEditorMode(EEditorMode p_newEditorMode)
{
	m_editorMode = p_newEditorMode;
	EditorModeChangedEvent.Invoke(m_editorMode);
}

void AmberEngine::Core::EditorAction::StartPlaying()
{
	if (m_editorMode == EEditorMode::EDIT)
	{
		//m_context.scriptInterpreter->RefreshAll();
		EDITOR_PANEL(UI::Panels::Inspector, "Inspector").Refresh();

		//if (m_context.scriptInterpreter->IsOk())
		//{
			PlayEvent.Invoke();
			m_sceneBackup.Clear();
			tinyxml2::XMLNode* node = m_sceneBackup.NewElement("root");
			m_sceneBackup.InsertFirstChild(node);
			m_context.sceneManager.GetCurrentScene()->OnSerialize(m_sceneBackup, node);
			m_panelsManager.GetPanelAs<UI::Panels::GameView>("Game View").Focus();
			m_context.sceneManager.GetCurrentScene()->Play();
			SetEditorMode(EEditorMode::PLAY);
		//}
	}
	else
	{
		//m_context.audioEngine->Unsuspend();
		SetEditorMode(EEditorMode::PLAY);
	}
}

void AmberEngine::Core::EditorAction::PauseGame()
{
	SetEditorMode(EEditorMode::PAUSE);
	//m_context.audioEngine->Suspend();
}

void AmberEngine::Core::EditorAction::StopPlaying()
{
	if (m_editorMode != EEditorMode::EDIT)
	{
		ImGui::GetIO().DisableMouseUpdate = false;
		m_context.window->SetCursorMode(AmberEngine::Context::ECursorMode::NORMAL);
		SetEditorMode(EEditorMode::EDIT);
		bool loadedFromDisk = m_context.sceneManager.IsCurrentSceneLoadedFromDisk();
		std::string sceneSourcePath = m_context.sceneManager.GetCurrentSceneSourcePath();

		int64_t focusedActorID = -1;

		if (auto targetActor = EDITOR_PANEL(UI::Panels::Inspector, "Inspector").GetTargetActor())
			focusedActorID = targetActor->GetID();

		m_context.sceneManager.LoadSceneFromMemory(m_sceneBackup);
		if (loadedFromDisk)
			m_context.sceneManager.StoreCurrentSceneSourcePath(sceneSourcePath); // To bo able to save or reload the scene whereas the scene is loaded from memory (Supposed to have no path)
		m_sceneBackup.Clear();
		EDITOR_PANEL(UI::Panels::SceneView, "Scene View").Focus();
		if (auto actorInstance = m_context.sceneManager.GetCurrentScene()->FindActorByID(focusedActorID))
			EDITOR_PANEL(UI::Panels::Inspector, "Inspector").FocusActor(*actorInstance);
	}
}

void AmberEngine::Core::EditorAction::NextFrame()
{
	if (m_editorMode == EEditorMode::PLAY || m_editorMode == EEditorMode::PAUSE)
		SetEditorMode(EEditorMode::FRAME_BY_FRAME);
}

std::string AmberEngine::Core::EditorAction::FindDuplicatedActorUniqueName(ECS::Actor& p_duplicated, ECS::Actor& p_newActor, SceneSystem::Scene& p_scene)
{
	const auto parent = p_newActor.GetParent();
	const auto adjacentActors = parent ? parent->GetChildren() : p_scene.GetActors();

	auto availabilityChecker = [&parent, &adjacentActors](std::string target) -> bool
	{
		const auto isActorNameTaken = [&target, parent](auto actor) { return (parent || !actor->GetParent()) && actor->GetName() == target; };
		return std::find_if(adjacentActors.begin(), adjacentActors.end(), isActorNameTaken) == adjacentActors.end();
	};

	return Tools::Utils::String::GenerateUnique(p_duplicated.GetName(), availabilityChecker);
}
