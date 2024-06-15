#include "Amberpch.h"

#include "AmberEngine/Core/EditorAction.h"

#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"
#include "AmberEngine/Core/ECS/Components/CMaterialRenderer.h"
#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/UI/Panels/Views/SceneView.h"
#include "AmberEngine/Core/SceneSystem/Scene.h"
#include "AmberEngine/Tools/Utils/String.h"
#include "AmberEngine/UI/Panels/Inspector.h"

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
	//tinyxml2::XMLDocument doc;
	//tinyxml2::XMLNode* actorsRoot = doc.NewElement("actors");
	//p_toDuplicate.OnSerialize(doc, actorsRoot);
	//auto& newActor = CreateEmptyActor(false);
	//int64_t idToUse = newActor.GetID();
	//tinyxml2::XMLElement* currentActor = actorsRoot->FirstChildElement("actor");
	//newActor.OnDeserialize(doc, currentActor);
	//
	//newActor.SetID(idToUse);
	//
	//if (p_forcedParent)
	//	newActor.SetParent(*p_forcedParent);
	//else
	//{
	//	auto currentScene = m_context.sceneManager.GetCurrentScene();
	//
	//	if (newActor.GetParentID() > 0)
	//	{
	//		if (auto found = currentScene->FindActorByID(newActor.GetParentID()); found)
	//		{
	//			newActor.SetParent(*found);
	//		}
	//	}
	//
	//	const auto uniqueName = FindDuplicatedActorUniqueName(p_toDuplicate, newActor, *currentScene);
	//	newActor.SetName(uniqueName);
	//}
	//
	//if (p_focus)
	//	SelectActor(newActor);
	//
	//for (auto& child : p_toDuplicate.GetChildren())
	//	DuplicateActor(*child, &newActor, false);
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
