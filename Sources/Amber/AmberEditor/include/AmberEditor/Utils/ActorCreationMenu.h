#pragma once

#include <optional>

namespace AmberUI::Widgets
{
	class MenuList;
}

namespace AmberCore::ECS
{
	class Actor;
}

namespace AmberEditor::Utils
{
	class ActorCreationMenu
	{
	public:
		ActorCreationMenu() = delete;

		static void GenerateActorCreationMenu(AmberUI::Widgets::MenuList& p_menuList, AmberCore::ECS::Actor* p_parent = nullptr, const std::optional<std::function<void()>>& p_onItemClicked = {});
	};
}