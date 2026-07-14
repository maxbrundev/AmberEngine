#pragma once

#include <sol.hpp>

namespace AmberCore::Scripting
{
	class LuaActorBinder
	{
	public:
		static void BindActor(sol::state& p_luaState);
	};
}
