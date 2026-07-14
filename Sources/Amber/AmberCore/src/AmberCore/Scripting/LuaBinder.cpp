#include "Amberpch.h"

#include "AmberCore/Scripting/LuaBinder.h"

#include "AmberCore/Scripting/LuaMathsBinder.h"
#include "AmberCore/Scripting/LuaActorBinder.h"
#include "AmberCore/Scripting/LuaComponentBinder.h"
#include "AmberCore/Scripting/LuaGlobalsBinder.h"

void AmberCore::Scripting::LuaBinder::CallBinders(sol::state& p_luaState)
{
	LuaMathsBinder::BindMaths(p_luaState);
	LuaActorBinder::BindActor(p_luaState);
	LuaComponentBinder::BindComponent(p_luaState);
	LuaGlobalsBinder::BindGlobals(p_luaState);
}
