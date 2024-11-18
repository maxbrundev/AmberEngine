#include "Amberpch.h"

#include "AmberEngine/Core/ActorCreationMenu.h"

#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Core/ECS/Components/CDirectionalLight.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalBox.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalCapsule.h"
#include "AmberEngine/Core/ECS/Components/CPhysicalSphere.h"
#include "AmberEngine/Core/ECS/Components/CPointLight.h"
#include "AmberEngine/Core/ECS/Components/CSpotLight.h"
#include "AmberEngine/Tools/Global/ServiceLocator.h"
#include "AmberEngine/Tools/Utils/FunctionUtility.h"

template<class T>
std::function<void()> ActorWithComponentCreationHandler(AmberEngine::Core::ECS::Actor* p_parent, std::optional<std::function<void()>> p_onItemClicked)
{
	return AmberEngine::Tools::Utils::Combine(EDITOR_BIND(CreateMonoComponentActor<T>, true, p_parent), p_onItemClicked);
}

std::function<void()> ActorWithModelComponentCreationHandler(AmberEngine::Core::ECS::Actor* p_parent, const std::string& p_modelName, const std::optional<std::function<void()>>& p_onItemClicked)
{
	return AmberEngine::Tools::Utils::Combine(EDITOR_BIND(CreateActorWithModel, ":Models\\" + p_modelName + ".fbx", true, p_parent, p_modelName), p_onItemClicked);
}

void AmberEngine::Utils::ActorCreationMenu::GenerateActorCreationMenu(AmberEngine::UI::Widgets::MenuList& p_menuList, AmberEngine::Core::ECS::Actor* p_parent, const std::optional<std::function<void()>>& p_onItemClicked)
{
	p_menuList.CreateWidget<UI::Widgets::MenuItem>("Create Empty").ClickedEvent += AmberEngine::Tools::Utils::Combine(EDITOR_BIND(CreateEmptyActor, true, p_parent, ""), p_onItemClicked);

	auto& primitives = p_menuList.CreateWidget<UI::Widgets::MenuList>("Primitives");

	primitives.CreateWidget<UI::Widgets::MenuItem>("Cube").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Cube", p_onItemClicked);
	primitives.CreateWidget<UI::Widgets::MenuItem>("Sphere").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Sphere", p_onItemClicked);
	primitives.CreateWidget<UI::Widgets::MenuItem>("Cone").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Cone", p_onItemClicked);
	primitives.CreateWidget<UI::Widgets::MenuItem>("Cylinder").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Cylinder", p_onItemClicked);
	primitives.CreateWidget<UI::Widgets::MenuItem>("Plane").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Plane", p_onItemClicked);
	primitives.CreateWidget<UI::Widgets::MenuItem>("Gear").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Gear", p_onItemClicked);
	primitives.CreateWidget<UI::Widgets::MenuItem>("Helix").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Helix", p_onItemClicked);
	primitives.CreateWidget<UI::Widgets::MenuItem>("Pipe").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Pipe", p_onItemClicked);
	primitives.CreateWidget<UI::Widgets::MenuItem>("Pyramid").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Pyramid", p_onItemClicked);
	primitives.CreateWidget<UI::Widgets::MenuItem>("Torus").ClickedEvent += ActorWithModelComponentCreationHandler(p_parent, "Torus", p_onItemClicked);

	auto& physicals = p_menuList.CreateWidget<UI::Widgets::MenuList>("Physicals");

	physicals.CreateWidget<UI::Widgets::MenuItem>("Physical Box").ClickedEvent += ActorWithComponentCreationHandler<Core::ECS::Components::CPhysicalBox>(p_parent, p_onItemClicked);
	physicals.CreateWidget<UI::Widgets::MenuItem>("Physical Sphere").ClickedEvent += ActorWithComponentCreationHandler<Core::ECS::Components::CPhysicalSphere>(p_parent, p_onItemClicked);
	physicals.CreateWidget<UI::Widgets::MenuItem>("Physical Capsule").ClickedEvent += ActorWithComponentCreationHandler<Core::ECS::Components::CPhysicalCapsule>(p_parent, p_onItemClicked);

	auto& lights = p_menuList.CreateWidget<UI::Widgets::MenuList>("Lights");

	lights.CreateWidget<UI::Widgets::MenuItem>("Point").ClickedEvent += ActorWithComponentCreationHandler<Core::ECS::Components::CPointLight>(p_parent, p_onItemClicked);
	lights.CreateWidget<UI::Widgets::MenuItem>("Directional").ClickedEvent += ActorWithComponentCreationHandler<Core::ECS::Components::CDirectionalLight>(p_parent, p_onItemClicked);
	lights.CreateWidget<UI::Widgets::MenuItem>("Spot").ClickedEvent += ActorWithComponentCreationHandler<Core::ECS::Components::CSpotLight>(p_parent, p_onItemClicked);
	lights.CreateWidget<UI::Widgets::MenuItem>("Ambient Box").ClickedEvent += ActorWithComponentCreationHandler<Core::ECS::Components::CAmbientBoxLight>(p_parent, p_onItemClicked);
	lights.CreateWidget<UI::Widgets::MenuItem>("Ambient Sphere").ClickedEvent += ActorWithComponentCreationHandler<Core::ECS::Components::CAmbientSphereLight>(p_parent, p_onItemClicked);

	auto& others = p_menuList.CreateWidget<UI::Widgets::MenuList>("Others");

	others.CreateWidget<UI::Widgets::MenuItem>("Camera").ClickedEvent += ActorWithComponentCreationHandler<Core::ECS::Components::CCamera>(p_parent, p_onItemClicked);
}
