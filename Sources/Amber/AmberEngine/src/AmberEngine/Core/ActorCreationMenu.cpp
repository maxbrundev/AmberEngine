#include "Amberpch.h"

#include "AmberEngine/Core/ActorCreationMenu.h"

#include "AmberEngine/Core/EditorAction.h"
#include "AmberEngine/Core/ECS/Actor.h"
#include "AmberEngine/Tools/Global/ServiceLocator.h"

std::function<void()> Combine(std::function<void()> p_function, const std::optional<std::function<void()>>& p_OptionalFunction)
{
	if (p_OptionalFunction.has_value())
	{
		return [=]()
		{
			p_function();
			p_OptionalFunction.value()();
		};
	}

	return p_function;
}


std::function<void()> ActorWithModelComponentCreationHandler(AmberEngine::Core::ECS::Actor* p_parent, const std::string& p_modelName, const std::optional<std::function<void()>>& p_onItemClicked)
{
	return Combine(EDITOR_BIND(CreateActorWithModel, ":Models\\" + p_modelName + ".fbx", true, p_parent, p_modelName), p_onItemClicked);
}

void AmberEngine::Utils::ActorCreationMenu::GenerateActorCreationMenu(AmberEngine::UI::Widgets::MenuList& p_menuList, AmberEngine::Core::ECS::Actor* p_parent, const std::optional<std::function<void()>>& p_onItemClicked)
{
	p_menuList.CreateWidget<UI::Widgets::MenuItem>("Create Empty").ClickedEvent += Combine(EDITOR_BIND(CreateEmptyActor, true, p_parent, ""), p_onItemClicked);

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
}
