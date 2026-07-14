#include "Amberpch.h"

#include "AmberEditor/Utils/ActorCreationMenu.h"

#include "AmberEditor/Core/EditorAction.h"
#include "AmberCore/ECS/Actor.h"
#include "AmberCore/ECS/Components/CDirectionalLight.h"
#include "AmberCore/ECS/Components/CPhysicalBox.h"
#include "AmberCore/ECS/Components/CPhysicalCapsule.h"
#include "AmberCore/ECS/Components/CPhysicalSphere.h"
#include "AmberCore/ECS/Components/CPointLight.h"
#include "AmberCore/ECS/Components/CSpotLight.h"
#include "AmberTools/Global/ServiceLocator.h"
#include "AmberTools/Utils/FunctionUtility.h"

template<class T>
std::function<void()> ActorWithComponentCreationHandler(AmberCore::ECS::Actor* p_parent, std::optional<std::function<void()>> p_onItemClicked)
{
	return AmberTools::Utils::Combine(EDITOR_BIND(CreateMonoComponentActor<T>, true, p_parent), p_onItemClicked);
}

std::function<void()> ActorWithModelComponentCreationHandler(AmberCore::ECS::Actor* p_parent, const std::string& p_modelName, const std::optional<std::function<void()>>& p_onItemClicked)
{
	return AmberTools::Utils::Combine(EDITOR_BIND(CreateActorWithModel, ":Models\\" + p_modelName + ".fbx", true, p_parent, p_modelName), p_onItemClicked);
}

void AmberEditor::Utils::ActorCreationMenu::GenerateActorCreationMenu(AmberUI::Widgets::MenuList& p_menuList, AmberCore::ECS::Actor* p_parent, const std::optional<std::function<void()>>& p_onItemClicked)
{
	p_menuList.CreateWidget<AmberUI::Widgets::MenuItem>("Create Empty").ClickedEvent += AmberTools::Utils::Combine(EDITOR_BIND(CreateEmptyActor, true, p_parent, ""), p_onItemClicked);

	auto& primitives = p_menuList.CreateWidget<AmberUI::Widgets::MenuList>("Primitives");

	primitives.CreateWidget<AmberUI::Widgets::MenuItem>("Cube").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Cube", p_onItemClicked);
	primitives.CreateWidget<AmberUI::Widgets::MenuItem>("Sphere").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Sphere", p_onItemClicked);
	primitives.CreateWidget<AmberUI::Widgets::MenuItem>("Cone").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Cone", p_onItemClicked);
	primitives.CreateWidget<AmberUI::Widgets::MenuItem>("Cylinder").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Cylinder", p_onItemClicked);
	primitives.CreateWidget<AmberUI::Widgets::MenuItem>("Plane").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Plane", p_onItemClicked);
	primitives.CreateWidget<AmberUI::Widgets::MenuItem>("Gear").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Gear", p_onItemClicked);
	primitives.CreateWidget<AmberUI::Widgets::MenuItem>("Helix").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Helix", p_onItemClicked);
	primitives.CreateWidget<AmberUI::Widgets::MenuItem>("Pipe").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Pipe", p_onItemClicked);
	primitives.CreateWidget<AmberUI::Widgets::MenuItem>("Pyramid").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Pyramid", p_onItemClicked);
	primitives.CreateWidget<AmberUI::Widgets::MenuItem>("Torus").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Torus", p_onItemClicked);

	auto& physicals = p_menuList.CreateWidget<AmberUI::Widgets::MenuList>("Physicals");

	physicals.CreateWidget<AmberUI::Widgets::MenuItem>("Physical Box").ClickedEvent += ActorWithComponentCreationHandler<AmberCore::ECS::Components::CPhysicalBox>(p_parent, p_onItemClicked);
	physicals.CreateWidget<AmberUI::Widgets::MenuItem>("Physical Sphere").ClickedEvent += ActorWithComponentCreationHandler<AmberCore::ECS::Components::CPhysicalSphere>(p_parent, p_onItemClicked);
	physicals.CreateWidget<AmberUI::Widgets::MenuItem>("Physical Capsule").ClickedEvent += ActorWithComponentCreationHandler<AmberCore::ECS::Components::CPhysicalCapsule>(p_parent, p_onItemClicked);

	auto& lights = p_menuList.CreateWidget<AmberUI::Widgets::MenuList>("Lights");

	lights.CreateWidget<AmberUI::Widgets::MenuItem>("Point").ClickedEvent += ActorWithComponentCreationHandler<AmberCore::ECS::Components::CPointLight>(p_parent, p_onItemClicked);
	lights.CreateWidget<AmberUI::Widgets::MenuItem>("Directional").ClickedEvent += ActorWithComponentCreationHandler<AmberCore::ECS::Components::CDirectionalLight>(p_parent, p_onItemClicked);
	lights.CreateWidget<AmberUI::Widgets::MenuItem>("Spot").ClickedEvent += ActorWithComponentCreationHandler<AmberCore::ECS::Components::CSpotLight>(p_parent, p_onItemClicked);
	lights.CreateWidget<AmberUI::Widgets::MenuItem>("Ambient Box").ClickedEvent += ActorWithComponentCreationHandler<AmberCore::ECS::Components::CAmbientBoxLight>(p_parent, p_onItemClicked);
	lights.CreateWidget<AmberUI::Widgets::MenuItem>("Ambient Sphere").ClickedEvent += ActorWithComponentCreationHandler<AmberCore::ECS::Components::CAmbientSphereLight>(p_parent, p_onItemClicked);

	auto& others = p_menuList.CreateWidget<AmberUI::Widgets::MenuList>("Others");

	others.CreateWidget<AmberUI::Widgets::MenuItem>("Camera").ClickedEvent += ActorWithComponentCreationHandler<AmberCore::ECS::Components::CCamera>(p_parent, p_onItemClicked);
}
