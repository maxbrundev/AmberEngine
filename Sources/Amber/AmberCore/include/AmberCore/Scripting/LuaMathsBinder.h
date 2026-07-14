#pragma once

#include <sol.hpp>

namespace AmberCore::Scripting
{
	class LuaMathsBinder
	{
	public:
		static void BindMaths(sol::state& p_luaState);
	};
}
