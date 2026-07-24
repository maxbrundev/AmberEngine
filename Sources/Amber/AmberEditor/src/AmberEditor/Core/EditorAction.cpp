#include "Amberpch.h"

#include "AmberEditor/Core/EditorAction.h"

#include <variant>

#include "AmberCore/ECS/Components/CModelRenderer.h"
#include "AmberCore/ECS/Components/CMaterialRenderer.h"
#include "AmberCore/Resources/Loaders/MaterialLoader.h"
#include "AmberRendering/Resources/Loaders/ShaderLoader.h"
#include "AmberTools/Global/ServiceLocator.h"
#include "AmberEditor/Panels/SceneView.h"
#include "AmberCore/SceneSystem/Scene.h"
#include "AmberTools/Utils/PathParser.h"
#include "AmberTools/Utils/String.h"
#include "AmberEditor/Panels/Inspector.h"
#include "AmberEditor/Panels/MaterialEditor.h"
#include "AmberEditor/Panels/AssetView.h"
#include "AmberEditor/Panels/GameView.h"
#include "AmberWindowing/Dialogs/OpenFileDialog.h"
#include "AmberWindowing/Dialogs/SaveFileDialog.h"
#include "AmberWindowing/Dialogs/MessageBox.h"

#include "AmberDebug/Logger.h"
#include "AmberTools/Utils/SystemCalls.h"

AmberEditor::Core::EditorAction::EditorAction(Context& p_context, EditorRenderer& p_editorRenderer, PanelsManager& p_panelsManager) :
	m_context(p_context),
	m_renderer(p_editorRenderer),
	m_panelsManager(p_panelsManager)
{
	AmberTools::Global::ServiceLocator::Provide<EditorAction>(*this);

	AmberCore::ECS::Components::CMaterialRenderer::MaterialFilesGenerationRequestedEvent += [this](const std::string& p_materialPath, const std::string& p_shaderPath)
	{
		GenerateModelMaterialFiles(p_materialPath, p_shaderPath);
	};

	AmberCore::ECS::Actor::DestroyEvent += [this](AmberCore::ECS::Actor& p_actor)
	{
		if (m_selectedActors.empty())
			return;

		std::vector<AmberCore::ECS::Actor*> selection = m_selectedActors;

		selection.erase(std::remove(selection.begin(), selection.end(), &p_actor), selection.end());

		if (!selection.empty())
			SelectActors(selection);
		else
			UnselectActor();
	};

	m_context.sceneManager.SceneUnloadEvent += [this]
	{
		if (!m_selectedActors.empty())
			UnselectActor();
	};
}

AmberEditor::Core::Context& AmberEditor::Core::EditorAction::GetContext()
{
	return m_context;
}

AmberEditor::Core::EditorRenderer& AmberEditor::Core::EditorAction::GetRenderer()
{
	return m_renderer;
}

AmberEditor::Core::PanelsManager& AmberEditor::Core::EditorAction::GetPanelsManager()
{
	return m_panelsManager;
}

void AmberEditor::Core::EditorAction::SetActorSpawnAtOrigin(bool p_value)
{
	if (p_value)
		m_actorSpawnMode = EActorSpawnMode::ORIGIN;
	else
		m_actorSpawnMode = EActorSpawnMode::FRONT;
}

void AmberEditor::Core::EditorAction::LoadEmptyScene()
{
	m_context.sceneManager.LoadEmptyLightedScene();
}

void AmberEditor::Core::EditorAction::SaveCurrentSceneTo(const std::string& p_path)
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* node = doc.NewElement("root");
	doc.InsertFirstChild(node);
	m_context.sceneManager.StoreCurrentSceneSourcePath(p_path);
	m_context.sceneManager.GetCurrentScene()->OnSerialize(doc, node);
	doc.SaveFile(p_path.c_str());
}

void AmberEditor::Core::EditorAction::LoadSceneFromDisk(const std::string& p_path, bool p_isAbsolute)
{
	m_context.sceneManager.LoadScene(p_path, p_isAbsolute);
	m_panelsManager.GetPanelAs<AmberEditor::Panels::SceneView>("Scene View").Focus();
}

bool AmberEditor::Core::EditorAction::IsCurrentSceneLoadedFromDisk() const
{
	return m_context.sceneManager.IsCurrentSceneLoadedFromDisk();
}

void AmberEditor::Core::EditorAction::SaveSceneChanges()
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

void AmberEditor::Core::EditorAction::SaveAs()
{
	AmberWindowing::Dialogs::SaveFileDialog dialog("New Scene");
	dialog.SetInitialDirectory(m_context.projectAssetsPath + "New Scene");
	dialog.DefineExtension("Amber Scene", ".abscene");
	dialog.Show();

	if (dialog.HasSucceeded())
	{
		if (dialog.IsFileExisting())
		{
			AmberWindowing::Dialogs::MessageBox message("File already exists!", "The file \"" + dialog.GetSelectedFileName() + "\" already exists.\n\nUsing this file as the new home for your scene will erase any content stored in this file.\n\nAre you ok with that?", AmberWindowing::Dialogs::MessageBox::EMessageType::WARNING, AmberWindowing::Dialogs::MessageBox::EButtonLayout::YES_NO, true);
			switch (message.GetUserAction())
			{
			case AmberWindowing::Dialogs::MessageBox::EUserAction::YES: break;
			case AmberWindowing::Dialogs::MessageBox::EUserAction::NO: return;
			}
		}

		SaveCurrentSceneTo(dialog.GetSelectedFilePath());
	}
}

void AmberEditor::Core::EditorAction::SetActorSpawnMode(EActorSpawnMode p_value)
{
	m_actorSpawnMode = p_value;
}

void AmberEditor::Core::EditorAction::ResetToDefaultLayout()
{
	DelayAction([this]() {m_context.uiManager->LoadLayout("Config\\layout.ini"); });
}

void AmberEditor::Core::EditorAction::SaveLayout(const std::string& p_fileName)
{
	DelayAction([&]() {m_context.uiManager->SaveLayout(std::ref(p_fileName)); });
}

void AmberEditor::Core::EditorAction::SaveCurrentLayout()
{
	DelayAction([&]() {m_context.uiManager->SaveCurrentLayout(); });
}

void AmberEditor::Core::EditorAction::SetLayout(const std::string& p_fileName)
{
	DelayAction([&]() {m_context.uiManager->SetLayout(std::ref(p_fileName)); });
}

void AmberEditor::Core::EditorAction::SetSceneViewCameraSpeed(int p_value)
{
	EDITOR_PANEL(AmberEditor::Panels::SceneView, "Scene View").GetCameraController().SetSpeed((float)p_value);
}

int AmberEditor::Core::EditorAction::GetSceneViewCameraSpeed()
{
	return (int)EDITOR_PANEL(AmberEditor::Panels::SceneView, "Scene View").GetCameraController().GetSpeed();
}

void AmberEditor::Core::EditorAction::SetAssetViewCameraSpeed(int p_value)
{
	EDITOR_PANEL(AmberEditor::Panels::AssetView, "Asset View").GetCameraController().SetSpeed((float)p_value);
}

int AmberEditor::Core::EditorAction::GetAssetViewCameraSpeed()
{
	return (int)EDITOR_PANEL(AmberEditor::Panels::AssetView, "Asset View").GetCameraController().GetSpeed();
}

void AmberEditor::Core::EditorAction::ResetAssetViewCameraPosition()
{
	EDITOR_PANEL(AmberEditor::Panels::AssetView, "Asset View").GetCameraController().SetPosition({ -10.0f, 4.0f, 10.0f });
}

void AmberEditor::Core::EditorAction::ResetSceneViewCameraPosition()
{
	EDITOR_PANEL(AmberEditor::Panels::SceneView, "Scene View").GetCameraController().SetPosition({ -10.0f, 4.0f, 10.0f });
}

glm::vec3 AmberEditor::Core::EditorAction::ComputeActorSpawnPoint(float p_distanceToCamera)
{
	auto& sceneView = m_panelsManager.GetPanelAs<AmberEditor::Panels::SceneView>("Scene View");
	return sceneView.GetCameraPosition() + sceneView.GetCameraRotation() * glm::vec3(0.0f, 0.0f, 1.0f) * p_distanceToCamera;
}

AmberCore::ECS::Actor& AmberEditor::Core::EditorAction::CreateEmptyActor(bool p_focusOnCreation, AmberCore::ECS::Actor* p_parent, const std::string& p_name)
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

AmberCore::ECS::Actor& AmberEditor::Core::EditorAction::CreateActorWithModel(const std::string& p_path, bool p_focusOnCreation, AmberCore::ECS::Actor* p_parent, const std::string& p_name)
{
	auto& instance = CreateEmptyActor(false, p_parent, p_name);

	auto& modelRenderer = instance.AddComponent<AmberCore::ECS::Components::CModelRenderer>();

	const auto model = m_context.modelManager[p_path];
	if (model)
		modelRenderer.SetModel(model);

	auto& materialRenderer = instance.AddComponent<AmberCore::ECS::Components::CMaterialRenderer>();
	const auto material = m_context.materialManager[":Materials\\Default.abmat"];
	if (material)
		materialRenderer.FillWithMaterial(*material);

	if (p_focusOnCreation)
		SelectActor(instance);

	return instance;
}

bool AmberEditor::Core::EditorAction::DestroyActor(AmberCore::ECS::Actor& p_actor)
{
	p_actor.MarkAsDestroy();

	return true;
}

void AmberEditor::Core::EditorAction::DestroySelectedActors()
{
	for (auto m_selected_actor : m_selectedActors)
	{
		m_selected_actor->MarkAsDestroy();
	}

	UnselectActors();
}

void AmberEditor::Core::EditorAction::DuplicateActor(AmberCore::ECS::Actor& p_toDuplicate, AmberCore::ECS::Actor* p_forcedParent, bool p_focus)
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

void AmberEditor::Core::EditorAction::SelectActor(AmberCore::ECS::Actor& p_target)
{
	SelectActors({ &p_target });
}

void AmberEditor::Core::EditorAction::AddActorToSelection(AmberCore::ECS::Actor& p_target)
{
	if (std::find(m_selectedActors.begin(), m_selectedActors.end(), &p_target) != m_selectedActors.end())
	{
		return;
	}

	m_selectedActors.push_back(&p_target);

	UpdateActorSelection();
}

void AmberEditor::Core::EditorAction::RemoveActorFromSelection(AmberCore::ECS::Actor& p_target)
{
	if (std::find(m_selectedActors.begin(), m_selectedActors.end(), &p_target) == m_selectedActors.end())
	{
		return;
	}

	m_selectedActors.erase(std::remove(m_selectedActors.begin(), m_selectedActors.end(), &p_target), m_selectedActors.end());

	UpdateActorSelection();
}

void AmberEditor::Core::EditorAction::ToggleActorSelection(AmberCore::ECS::Actor& p_target)
{
	if (IsActorInSelection(p_target))
	{
		RemoveActorFromSelection(p_target);
	}
	else
	{
		AddActorToSelection(p_target);
	}
}

bool AmberEditor::Core::EditorAction::IsActorInSelection(AmberCore::ECS::Actor& p_target) const
{
	return std::find(m_selectedActors.begin(), m_selectedActors.end(), &p_target) != m_selectedActors.end();
}

void AmberEditor::Core::EditorAction::SelectActors(const std::vector<AmberCore::ECS::Actor*>& p_targets)
{
	if (m_selectedActors == p_targets)
		return;

	m_selectedActors = p_targets;

	UpdateActorSelection();
}

void AmberEditor::Core::EditorAction::UnselectActor()
{
	UnselectActors();
}

void AmberEditor::Core::EditorAction::UnselectActors()
{
	m_selectedActors.clear();

	UpdateActorSelection();
}

bool AmberEditor::Core::EditorAction::IsAnyActorSelected() const
{
	return !m_selectedActors.empty();
}

bool AmberEditor::Core::EditorAction::IsManyActorsSelected() const
{
	return m_selectedActors.size() > 1;
}

AmberCore::ECS::Actor& AmberEditor::Core::EditorAction::GetSelectedActor() const
{
	return *m_selectedActors.front();
}

const std::vector<AmberCore::ECS::Actor*>& AmberEditor::Core::EditorAction::GetSelectedActors() const
{
	return m_selectedActors;
}

void AmberEditor::Core::EditorAction::DelayAction(std::function<void()> p_action, uint32_t p_frames)
{
	m_delayedActions.emplace_back(p_frames + 1, p_action);
}

void AmberEditor::Core::EditorAction::ExecuteDelayedActions()
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

std::string AmberEditor::Core::EditorAction::GetRealPath(const std::string& p_path)
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

std::string AmberEditor::Core::EditorAction::GetResourcePath(const std::string& p_path, bool p_isFromEngine)
{
	std::string result = p_path;

	if (AmberTools::Utils::String::Replace(result, p_isFromEngine ? m_context.engineAssetsPath : m_context.projectAssetsPath, ""))
	{
		if (p_isFromEngine)
			result = ':' + result;
	}

	return result;
}

std::string AmberEditor::Core::EditorAction::GetScriptPath(const std::string& p_path)
{
	std::string result = p_path;

	AmberTools::Utils::String::Replace(result, m_context.projectScriptsPath, "");
	AmberTools::Utils::String::Replace(result, ".lua", "");

	return result;
}

void AmberEditor::Core::EditorAction::RefreshScripts()
{
	m_context.scriptInterpreter->RefreshAll();
	m_panelsManager.GetPanelAs<AmberEditor::Panels::Inspector>("Inspector").Refresh();

	if (m_context.scriptInterpreter->IsOk())
		AB_LOG_INFO("Scripts interpretation succeeded!");
}

void AmberEditor::Core::EditorAction::CompileShaders()
{
	for (auto shader : m_context.shaderManager.GetResources())
		AmberRendering::Resources::Loaders::ShaderLoader::Recompile(*shader.second, GetRealPath(shader.second->Path));
}

void AmberEditor::Core::EditorAction::SaveMaterials()
{
	for (auto& [id, material] : m_context.materialManager.GetResources())
		AmberCore::Resources::Loaders::MaterialLoader::Save(*material, GetRealPath(material->path));
}

void AmberEditor::Core::EditorAction::GenerateModelMaterialFiles(const std::string& p_materialPath, const std::string& p_shaderPath)
{
	std::string finalPath = m_context.projectAssetsPath + p_materialPath;

	if (std::filesystem::exists(finalPath))
		return;

	{
		const std::string containingFolder = AmberTools::Utils::PathParser::GetContainingFolder(finalPath);

		if (!std::filesystem::exists(containingFolder))
		{
			std::filesystem::create_directories(containingFolder);
		}

		std::ofstream outfile(finalPath);
		outfile << "<root><shader>" << p_shaderPath << "</shader></root>" << std::endl;
	}

	EDITOR_CONTEXT(materialManager)[EDITOR_EXEC(GetResourcePath(finalPath))];
}

bool AmberEditor::Core::EditorAction::ImportAsset(const std::string& p_initialDestinationDirectory)
{
	std::string modelFormats = "*.fbx;*.obj;";
	std::string textureFormats = "*.png;*.jpeg;*.jpg;*.tga";
	std::string shaderFormats = "*.glsl;";

	AmberWindowing::Dialogs::OpenFileDialog selectAssetDialog("Select an asset to import");
	selectAssetDialog.AddFileType("Any supported format", modelFormats + textureFormats + shaderFormats);
	selectAssetDialog.AddFileType("Model (.fbx, .obj)", modelFormats);
	selectAssetDialog.AddFileType("Texture (.png, .jpeg, .jpg, .tga)", textureFormats);
	selectAssetDialog.AddFileType("Shader (.glsl)", shaderFormats);
	selectAssetDialog.Show();

	if(selectAssetDialog.HasSucceeded())
	{
		std::string source = selectAssetDialog.GetSelectedFilePath();
		std::string extension = '.' + AmberTools::Utils::PathParser::GetExtension(source);
		std::string filename = selectAssetDialog.GetSelectedFileName();

		AmberWindowing::Dialogs::SaveFileDialog saveLocationDialog("Where to import?");
		saveLocationDialog.SetInitialDirectory(p_initialDestinationDirectory + filename);
		saveLocationDialog.DefineExtension(extension, extension);
		saveLocationDialog.Show();

		if (saveLocationDialog.HasSucceeded())
		{
			std::string destination = saveLocationDialog.GetSelectedFilePath();

			if (!std::filesystem::exists(destination) || AmberWindowing::Dialogs::MessageBox("File already exists", "The destination you have selected already exists, importing this file will erase the previous file content, are you sure about that?", AmberWindowing::Dialogs::MessageBox::EMessageType::WARNING, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK_CANCEL).GetUserAction() == AmberWindowing::Dialogs::MessageBox::EUserAction::OK)
			{
				std::filesystem::copy(source, destination, std::filesystem::copy_options::overwrite_existing);
				
				return true;
			}
		}
	}

	return false;
}

bool AmberEditor::Core::EditorAction::ImportAssetAtLocation(const std::string& p_destination)
{
	std::string modelFormats = "*.fbx;*.obj;";
	std::string textureFormats = "*.png;*.jpeg;*.jpg;*.tga;";
	std::string shaderFormats = "*.glsl;";

	AmberWindowing::Dialogs::OpenFileDialog selectAssetDialog("Select an asset to import");
	selectAssetDialog.AddFileType("Any supported format", modelFormats + textureFormats + shaderFormats);
	selectAssetDialog.AddFileType("Model (.fbx, .obj)", modelFormats);
	selectAssetDialog.AddFileType("Texture (.png, .jpeg, .jpg, .tga)", textureFormats);
	selectAssetDialog.AddFileType("Shader (.glsl)", shaderFormats);
	selectAssetDialog.Show();

	if (selectAssetDialog.HasSucceeded())
	{
		std::string source = selectAssetDialog.GetSelectedFilePath();
		std::string destination = p_destination + selectAssetDialog.GetSelectedFileName();

		if (!std::filesystem::exists(destination) || AmberWindowing::Dialogs::MessageBox("File already exists", "The destination you have selected already exists, importing this file will erase the previous file content, are you sure about that?", AmberWindowing::Dialogs::MessageBox::EMessageType::WARNING, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK_CANCEL).GetUserAction() == AmberWindowing::Dialogs::MessageBox::EUserAction::OK)
		{
			std::filesystem::copy(source, destination, std::filesystem::copy_options::overwrite_existing);
			return true;
		}
	}

	return false;
}

void AmberEditor::Core::EditorAction::PropagateFolderDestruction(std::string p_folderPath)
{
	for (auto& p : std::filesystem::recursive_directory_iterator(p_folderPath))
	{
		if (!p.is_directory())
		{
			PropagateFileRename(AmberTools::Utils::PathParser::MakeWindowsStyle(p.path().string()), "?");
		}
	}
}

void AmberEditor::Core::EditorAction::PropagateScriptRename(std::string p_previousName, std::string p_newName)
{
	p_previousName = GetScriptPath(p_previousName);
	p_newName = GetScriptPath(p_newName);

	if (auto currentScene = m_context.sceneManager.GetCurrentScene())
	{
		for (auto actor : currentScene->GetActors())
		{
			if (actor->RemoveBehaviour(p_previousName))
				actor->AddBehaviour(p_newName);
		}
	}

	PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, AmberTools::Utils::PathParser::EFileType::SCENE);

	EDITOR_PANEL(AmberEditor::Panels::Inspector, "Inspector").Refresh();
}

void AmberEditor::Core::EditorAction::PropagateFileRename(std::string p_previousName, std::string p_newName)
{
	p_previousName = GetResourcePath(p_previousName);
	p_newName = GetResourcePath(p_newName);

	if (p_newName != "?")
	{
		/* If not a real rename is asked (Not delete) */

		if (AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ModelManager>().MoveResource(p_previousName, p_newName))
		{
			AmberRendering::Resources::Model* resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ModelManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(AmberRendering::Resources::Model, Path)) = p_newName;
		}

		if (AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::TextureManager>().MoveResource(p_previousName, p_newName))
		{
			AmberRendering::Resources::Texture* resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::TextureManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(AmberRendering::Resources::Texture, Path)) = p_newName;
		}

		if (AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ShaderManager>().MoveResource(p_previousName, p_newName))
		{
			AmberRendering::Resources::Shader* resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ShaderManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(AmberRendering::Resources::Shader, Path)) = p_newName;
		}

		if (AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>().MoveResource(p_previousName, p_newName))
		{
			AmberCore::Resources::Material* resource = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>()[p_newName];
			*reinterpret_cast<std::string*>(reinterpret_cast<char*>(resource) + offsetof(AmberCore::Resources::Material, path)) = p_newName;
		}
	}
	else
	{
		if (auto texture = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::TextureManager>().GetResource(p_previousName, false))
		{
			for (auto[name, instance] : AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>().GetResources())
				if (instance)
					for (auto&[name, value] : instance->GetUniformsData())
						if (value.has_value() && value.type() == typeid(AmberRendering::Resources::Texture*))
							if (std::any_cast<AmberRendering::Resources::Texture*>(value) == texture)
								value = static_cast<AmberRendering::Resources::Texture*>(nullptr);

			auto& assetView = EDITOR_PANEL(AmberEditor::Panels::AssetView, "Asset View");
			auto assetViewRes = assetView.GetResource();
			if (auto pval = std::get_if<AmberRendering::Resources::Texture*>(&assetViewRes); pval && *pval)
				assetView.SetResource(static_cast<AmberRendering::Resources::Texture*>(nullptr));

			AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::TextureManager>().UnloadResource(p_previousName);
		}

		if (auto shader = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ShaderManager>().GetResource(p_previousName, false))
		{
			auto& materialEditor = EDITOR_PANEL(AmberEditor::Panels::MaterialEditor, "Material Editor");

			for (auto[name, instance] : AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>().GetResources())
				if (instance && instance->GetShader() == shader)
					instance->SetShader(nullptr);

			AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ShaderManager>().UnloadResource(p_previousName);
		}

		if (auto model = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ModelManager>().GetResource(p_previousName, false))
		{
			auto& assetView = EDITOR_PANEL(AmberEditor::Panels::AssetView, "Asset View");
			auto assetViewRes = assetView.GetResource();
			if (auto pval = std::get_if<AmberRendering::Resources::Model*>(&assetViewRes); pval && *pval)
				assetView.SetResource(static_cast<AmberRendering::Resources::Model*>(nullptr));

			if (auto currentScene = m_context.sceneManager.GetCurrentScene())
				for (auto actor : currentScene->GetActors())
					if (auto modelRenderer = actor->GetComponent<AmberCore::ECS::Components::CModelRenderer>(); modelRenderer && modelRenderer->GetModel() == model)
						modelRenderer->SetModel(nullptr);

			AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::ModelManager>().UnloadResource(p_previousName);
		}

		if (auto material = AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>().GetResource(p_previousName, false))
		{
			auto& materialEditor = EDITOR_PANEL(AmberEditor::Panels::MaterialEditor, "Material Editor");

			if (materialEditor.GetTarget() == material)
				materialEditor.RemoveTarget();

			auto& assetView = EDITOR_PANEL(AmberEditor::Panels::AssetView, "Asset View");
			auto assetViewRes = assetView.GetResource();
			if (auto pval = std::get_if<AmberCore::Resources::Material*>(&assetViewRes); pval && *pval)
				assetView.SetResource(static_cast<AmberCore::Resources::Material*>(nullptr));

			if (auto currentScene = m_context.sceneManager.GetCurrentScene())
				for (auto actor : currentScene->GetActors())
					if (auto materialRenderer = actor->GetComponent<AmberCore::ECS::Components::CMaterialRenderer>(); materialRenderer)
						materialRenderer->RemoveMaterialByInstance(*material);

			AmberTools::Global::ServiceLocator::Get<AmberCore::ResourceManagement::MaterialManager>().UnloadResource(p_previousName);
		}
	}

	switch (AmberTools::Utils::PathParser::GetFileType(p_previousName))
	{
	case AmberTools::Utils::PathParser::EFileType::MATERIAL:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, AmberTools::Utils::PathParser::EFileType::SCENE);
		break;
	case AmberTools::Utils::PathParser::EFileType::MODEL:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, AmberTools::Utils::PathParser::EFileType::SCENE);
		break;
	case AmberTools::Utils::PathParser::EFileType::SHADER:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, AmberTools::Utils::PathParser::EFileType::MATERIAL);
		break;
	case AmberTools::Utils::PathParser::EFileType::TEXTURE:
		PropagateFileRenameThroughSavedFilesOfType(p_previousName, p_newName, AmberTools::Utils::PathParser::EFileType::MATERIAL);
		break;
	}

	EDITOR_PANEL(AmberEditor::Panels::Inspector, "Inspector").Refresh();
	EDITOR_PANEL(AmberEditor::Panels::MaterialEditor, "Material Editor").Refresh();
}

void AmberEditor::Core::EditorAction::PropagateFileRenameThroughSavedFilesOfType(const std::string& p_previousName,
	const std::string& p_newName, AmberTools::Utils::PathParser::EFileType p_fileType)
{
	for (auto& entry : std::filesystem::recursive_directory_iterator(m_context.projectAssetsPath))
	{
		if (AmberTools::Utils::PathParser::GetFileType(entry.path().string()) == p_fileType)
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
					AmberTools::Utils::String::Replace(line, wordToReplace, wordToReplaceWith);
				}

				out.close(); in.close();
			}

			std::filesystem::copy_file("TEMP", entry.path(), std::filesystem::copy_options::overwrite_existing);
			std::filesystem::remove("TEMP");
		}
	}
}

void AmberEditor::Core::EditorAction::PropagateFolderRename(std::string p_previousName, std::string p_newName)
{
	p_previousName = AmberTools::Utils::PathParser::MakeNonWindowsStyle(p_previousName);
	p_newName = AmberTools::Utils::PathParser::MakeNonWindowsStyle(p_newName);

	for (auto& p : std::filesystem::recursive_directory_iterator(p_newName))
	{
		if (!p.is_directory())
		{
			std::string newFileName = AmberTools::Utils::PathParser::MakeNonWindowsStyle(p.path().string());
			std::string previousFileName;

			for (char c : newFileName)
			{
				previousFileName += c;
				if (previousFileName == p_newName)
					previousFileName = p_previousName;
			}

			PropagateFileRename(AmberTools::Utils::PathParser::MakeWindowsStyle(previousFileName), AmberTools::Utils::PathParser::MakeWindowsStyle(newFileName));
		}
	}
}

AmberEditor::Core::EditorAction::EEditorMode AmberEditor::Core::EditorAction::GetCurrentEditorMode() const
{
	return m_editorMode;
}

void AmberEditor::Core::EditorAction::SetEditorMode(EEditorMode p_newEditorMode)
{
	m_editorMode = p_newEditorMode;
	EditorModeChangedEvent.Invoke(m_editorMode);
}

void AmberEditor::Core::EditorAction::StartPlaying()
{
	if (m_editorMode == EEditorMode::EDIT)
	{
		m_context.scriptInterpreter->RefreshAll();
		EDITOR_PANEL(AmberEditor::Panels::Inspector, "Inspector").Refresh();

		if (m_context.scriptInterpreter->IsOk())
		{
			PlayEvent.Invoke();
			m_sceneBackup.Clear();
			tinyxml2::XMLNode* node = m_sceneBackup.NewElement("root");
			m_sceneBackup.InsertFirstChild(node);
			m_context.sceneManager.GetCurrentScene()->OnSerialize(m_sceneBackup, node);
			m_panelsManager.GetPanelAs<AmberEditor::Panels::GameView>("Game View").Focus();
			m_context.sceneManager.GetCurrentScene()->Play();
			SetEditorMode(EEditorMode::PLAY);
		}
	}
	else
	{
		m_context.audioEngine->Unsuspend();
		SetEditorMode(EEditorMode::PLAY);
	}
}

void AmberEditor::Core::EditorAction::PauseGame()
{
	SetEditorMode(EEditorMode::PAUSE);
	m_context.audioEngine->Suspend();
}

void AmberEditor::Core::EditorAction::StopPlaying()
{
	if (m_editorMode != EEditorMode::EDIT)
	{
		ImGui::GetIO().DisableMouseUpdate = false;
		m_context.window->SetCursorMode(AmberWindowing::Context::ECursorMode::NORMAL);
		SetEditorMode(EEditorMode::EDIT);
		bool loadedFromDisk = m_context.sceneManager.IsCurrentSceneLoadedFromDisk();
		std::string sceneSourcePath = m_context.sceneManager.GetCurrentSceneSourcePath();

		int64_t focusedActorID = -1;

		if (auto targetActor = EDITOR_PANEL(AmberEditor::Panels::Inspector, "Inspector").GetTargetActor())
			focusedActorID = targetActor->GetID();

		m_context.sceneManager.LoadSceneFromMemory(m_sceneBackup);
		if (loadedFromDisk)
			m_context.sceneManager.StoreCurrentSceneSourcePath(sceneSourcePath); // To bo able to save or reload the scene whereas the scene is loaded from memory (Supposed to have no path)
		m_sceneBackup.Clear();
		EDITOR_PANEL(AmberEditor::Panels::SceneView, "Scene View").Focus();
		if (auto actorInstance = m_context.sceneManager.GetCurrentScene()->FindActorByID(focusedActorID))
			EDITOR_PANEL(AmberEditor::Panels::Inspector, "Inspector").FocusActor(*actorInstance);
	}
}

void AmberEditor::Core::EditorAction::NextFrame()
{
	if (m_editorMode == EEditorMode::PLAY || m_editorMode == EEditorMode::PAUSE)
		SetEditorMode(EEditorMode::FRAME_BY_FRAME);
}

std::string AmberEditor::Core::EditorAction::FindDuplicatedActorUniqueName(AmberCore::ECS::Actor& p_duplicated, AmberCore::ECS::Actor& p_newActor, AmberCore::SceneSystem::Scene& p_scene)
{
	const auto parent = p_newActor.GetParent();
	const auto adjacentActors = parent ? parent->GetChildren() : p_scene.GetActors();

	auto availabilityChecker = [&parent, &adjacentActors](std::string target) -> bool
	{
		const auto isActorNameTaken = [&target, parent](auto actor) { return (parent || !actor->GetParent()) && actor->GetName() == target; };
		return std::find_if(adjacentActors.begin(), adjacentActors.end(), isActorNameTaken) == adjacentActors.end();
	};

	return AmberTools::Utils::String::GenerateUnique(p_duplicated.GetName(), availabilityChecker);
}

void AmberEditor::Core::EditorAction::UpdateActorSelection()
{
	if (m_selectedActors.size() == 1)
	{
		EDITOR_PANEL(AmberEditor::Panels::Inspector, "Inspector").FocusActor(*m_selectedActors[0]);
	}
	else
	{
		EDITOR_PANEL(AmberEditor::Panels::Inspector, "Inspector").UnFocus();
	}

	EDITOR_EVENT(ActorSelectionEvent).Invoke();
}

std::string AmberEditor::Core::EditorAction::SelectBuildFolder()
{
	AmberWindowing::Dialogs::SaveFileDialog dialog("Build location");
	dialog.DefineExtension("Game Build", "..");
	dialog.Show();

	if (dialog.HasSucceeded())
	{
		std::string result = dialog.GetSelectedFilePath();
		result = std::string(result.data(), result.data() + result.size() - std::string("..").size()) + "\\";

		if (!std::filesystem::exists(result))
			return result;

		AmberWindowing::Dialogs::MessageBox message("Folder already exists!", "The folder \"" + result + "\" already exists.\n\nPlease select another location and try again", AmberWindowing::Dialogs::MessageBox::EMessageType::WARNING, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK, true);
	}

	return "";
}

void AmberEditor::Core::EditorAction::Build(bool p_autoRun, bool p_tempFolder)
{
	std::string destinationFolder;

	if (p_tempFolder)
	{
		destinationFolder = std::string(getenv("APPDATA")) + "\\AmberEngine\\Editor\\TempBuild\\";

		try
		{
			std::filesystem::remove_all(destinationFolder);
		}
		catch (std::filesystem::filesystem_error error)
		{
			AmberWindowing::Dialogs::MessageBox message("Temporary build failed", "The temporary folder is currently being used by another process", AmberWindowing::Dialogs::MessageBox::EMessageType::ERROR, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK, true);
			return;
		}
	}
	else
	{
		destinationFolder = SelectBuildFolder();

		if (destinationFolder.empty())
			return;
	}

	BuildAtLocation(m_context.projectSettings.GetOrDefault<bool>("dev_build", true) ? "Development" : "Shipping", destinationFolder, p_autoRun);
}

void AmberEditor::Core::EditorAction::BuildAtLocation(const std::string& p_configuration, const std::string p_buildPath, bool p_autoRun)
{
	std::string buildPath(p_buildPath);
	std::string executableName = m_context.projectSettings.GetOrDefault<std::string>("executable_name", "Game") + ".exe";

	const std::string normalizedBuildPath = AmberTools::Utils::PathParser::MakeWindowsStyle(buildPath);
	const std::string normalizedProjectPath = AmberTools::Utils::PathParser::MakeWindowsStyle(m_context.projectPath);

	if (normalizedBuildPath.find(normalizedProjectPath) == 0)
	{
		AB_LOG_ERROR("Cannot build inside the project folder (\"" + buildPath + "\")");
		AmberWindowing::Dialogs::MessageBox message("Build failed", "The build location can not be inside the project folder.\n\nPlease select another location and try again", AmberWindowing::Dialogs::MessageBox::EMessageType::WARNING, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK, true);
		return;
	}

	bool failed = false;

	AB_LOG_INFO("Preparing to build at location: \"" + buildPath + "\"");

	std::filesystem::remove_all(buildPath);

	if (std::filesystem::create_directory(buildPath))
	{
		AB_LOG_INFO("Build directory created");

		if (std::filesystem::create_directory(buildPath + "Data\\"))
		{
			AB_LOG_INFO("Data directory created");

			if (std::filesystem::create_directory(buildPath + "Data\\User\\"))
			{
				AB_LOG_INFO("Data\\User directory created");

				std::error_code err;

				std::filesystem::copy(m_context.projectFilePath, buildPath + "Data\\User\\Game.ini", err);

				if (!err)
				{
					AB_LOG_INFO("Data\\User\\Game.ini file generated");

					std::filesystem::copy(m_context.projectAssetsPath, buildPath + "Data\\User\\Assets\\", std::filesystem::copy_options::recursive, err);

					if (!err)
					{
						AB_LOG_INFO("Data\\User\\Assets\\ directory copied");

						if (std::filesystem::exists(m_context.projectScriptsPath))
						{
							std::filesystem::copy(m_context.projectScriptsPath, buildPath + "Data\\User\\Scripts\\", std::filesystem::copy_options::recursive, err);

							if (!err)
							{
								AB_LOG_INFO("Data\\User\\Scripts\\ directory copied");
							}
							else
							{
								AB_LOG_ERROR("Data\\User\\Scripts\\ directory failed to copy: " + err.message());
								failed = true;
							}
						}

						if (!failed)
						{
							std::filesystem::copy(m_context.engineAssetsPath, buildPath + "Data\\Engine\\", std::filesystem::copy_options::recursive, err);

							if (!err)
							{
								AB_LOG_INFO("Data\\Engine\\ directory copied");
							}
							else
							{
								AB_LOG_ERROR("Data\\Engine\\ directory failed to copy: " + err.message());
								failed = true;
							}
						}
					}
					else
					{
						AB_LOG_ERROR("Data\\User\\Assets\\ directory failed to copy: " + err.message());
						failed = true;
					}
				}
				else
				{
					AB_LOG_ERROR("Data\\User\\Game.ini file failed to generate: " + err.message());
					failed = true;
				}

				std::string builderFolder = "Builder\\" + p_configuration + "\\";

				if (!failed)
				{
					if (std::filesystem::exists(builderFolder))
					{
						std::error_code copyError;

						std::filesystem::copy(builderFolder, buildPath, copyError);

						if (!copyError)
						{
							AB_LOG_INFO("Builder data (Dlls and executable) copied");

							std::filesystem::rename(buildPath + "AmberGame.exe", buildPath + executableName, copyError);

							if (!copyError)
							{
								AB_LOG_INFO("Game executable renamed to " + executableName);

								if (p_autoRun)
								{
									std::string exePath = buildPath + executableName;
									AB_LOG_INFO("Launching the game at location: \"" + exePath + "\"");

									if (std::filesystem::exists(exePath))
										AmberTools::Utils::SystemCalls::OpenFile(exePath, buildPath);
									else
									{
										AB_LOG_ERROR("Failed to run the game: Executable not found");
										failed = true;
									}
								}
							}
							else
							{
								AB_LOG_ERROR("Game executable failed to rename: " + copyError.message());
								failed = true;
							}
						}
						else
						{
							AB_LOG_ERROR("Builder data (Dlls and executable) failed to copy: " + copyError.message());
							failed = true;
						}
					}
					else
					{
						AB_LOG_ERROR("Builder data (Dlls and executable) not found (Missing \"" + builderFolder + "\")");
						failed = true;
					}
				}
			}
		}
	}

	if (failed)
	{
		std::filesystem::remove_all(buildPath);
		AmberWindowing::Dialogs::MessageBox message("Build failed", "Check the console for more information", AmberWindowing::Dialogs::MessageBox::EMessageType::ERROR, AmberWindowing::Dialogs::MessageBox::EButtonLayout::OK, true);
	}
	else
	{
		AB_LOG_INFO("Build succeeded at location: \"" + buildPath + "\"");
	}
}
