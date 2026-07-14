#pragma once

#include <sol.hpp>

namespace AmberCore::Scripting
{
	class LuaComponentBinder
	{
	public:
		static void BindComponent(sol::state& p_luaState);
	};
}
