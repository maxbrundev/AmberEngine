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
	/**
	* Class exposing tools to generate an actor creation menu
	*/
	class ActorCreationMenu
	{
	public:
		/**
		* Disabled constructor
		*/
		ActorCreationMenu() = delete;

		/**
		* Generates an actor creation menu under the given MenuList item.
		* Also handles custom additionnal OnClick callback
		* @param p_menuList
		* @param p_parent
		* @param p_onItemClicked
		*/
		static void GenerateActorCreationMenu(AmberEngine::UI::Widgets::MenuList& p_menuList, AmberEngine::Core::ECS::Actor* p_parent = nullptr, const std::optional<std::function<void()>>& p_onItemClicked = {});
	};
}