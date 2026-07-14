#pragma once

#include <sol.hpp>

namespace AmberCore::Scripting
{
	class LuaBinder
	{
	public:
		static void CallBinders(sol::state& p_luaState);
	};
}
