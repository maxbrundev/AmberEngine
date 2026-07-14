#pragma once

#include <sol.hpp>

namespace AmberCore::Scripting
{
	class LuaGlobalsBinder
	{
	public:
		static void BindGlobals(sol::state& p_luaState);
	};
}
