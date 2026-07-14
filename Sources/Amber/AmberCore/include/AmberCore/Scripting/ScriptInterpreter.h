#pragma once

#include <vector>

#include <sol.hpp>

#include "AmberCore/ECS/Components/Behaviour.h"

namespace AmberCore::Scripting
{
	class ScriptInterpreter
	{
	public:
		ScriptInterpreter(const std::string& p_scriptRootFolder);
		~ScriptInterpreter();

		void CreateLuaContextAndBindGlobals();
		void DestroyLuaContext();

		void Consider(ECS::Components::Behaviour* p_toConsider);
		void Unconsider(ECS::Components::Behaviour* p_toUnconsider);

		void RefreshAll();
		bool IsOk() const;

	private:
		std::unique_ptr<sol::state> m_luaState;
		std::string m_scriptRootFolder;
		std::vector<ECS::Components::Behaviour*> m_behaviours;
		bool m_isOk;
	};
}
