#pragma once

#include <optional>

namespace AmberEngine::UI::Widgets
{
	class MenuList;
}

namespace AmberEngine::Core::ECS
{
	class Actor;
}

namespace AmberEngine::Utils
{
	class ActorCreationMenu
	{
	public:
		ActorCreationMenu() = delete;

		static void GenerateActorCreationMenu(AmberEngine::UI::Widgets::MenuList& p_menuList, AmberEngine::Core::ECS::Actor* p_parent = nullptr, const std::optional<std::function<void()>>& p_onItemClicked = {});
	};
}