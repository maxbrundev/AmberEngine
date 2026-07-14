#include "Amberpch.h"

#include "AmberCore/Scripting/ScriptInterpreter.h"

#include "AmberDebug/Logger.h"

#include "AmberCore/Scripting/LuaBinder.h"

AmberCore::Scripting::ScriptInterpreter::ScriptInterpreter(const std::string& p_scriptRootFolder) :
	m_scriptRootFolder(p_scriptRootFolder)
{
	CreateLuaContextAndBindGlobals();

	ECS::Components::Behaviour::CreatedEvent += std::bind(&ScriptInterpreter::Consider, this, std::placeholders::_1);
	ECS::Components::Behaviour::DestroyedEvent += std::bind(&ScriptInterpreter::Unconsider, this, std::placeholders::_1);
}

AmberCore::Scripting::ScriptInterpreter::~ScriptInterpreter()
{
	DestroyLuaContext();
}

void AmberCore::Scripting::ScriptInterpreter::CreateLuaContextAndBindGlobals()
{
	if (!m_luaState)
	{
		m_luaState = std::make_unique<sol::state>();
		m_luaState->open_libraries(sol::lib::base, sol::lib::math);
		LuaBinder::CallBinders(*m_luaState);
		m_isOk = true;

		std::for_each(m_behaviours.begin(), m_behaviours.end(), [this](ECS::Components::Behaviour* p_behaviour)
		{
			if (!p_behaviour->RegisterToLuaContext(*m_luaState, m_scriptRootFolder))
				m_isOk = false;
		});

		if (!m_isOk)
			AB_LOG_ERROR("Script interpreter failed to register scripts. Check your lua scripts");
	}
}

void AmberCore::Scripting::ScriptInterpreter::DestroyLuaContext()
{
	if (m_luaState)
	{
		std::for_each(m_behaviours.begin(), m_behaviours.end(), [](ECS::Components::Behaviour* p_behaviour)
		{
			p_behaviour->UnregisterFromLuaContext();
		});

		m_luaState.reset();
		m_isOk = false;
	}
}

void AmberCore::Scripting::ScriptInterpreter::Consider(ECS::Components::Behaviour* p_toConsider)
{
	if (m_luaState)
	{
		m_behaviours.push_back(p_toConsider);

		if (!p_toConsider->RegisterToLuaContext(*m_luaState, m_scriptRootFolder))
			m_isOk = false;
	}
}

void AmberCore::Scripting::ScriptInterpreter::Unconsider(ECS::Components::Behaviour* p_toUnconsider)
{
	if (m_luaState)
		p_toUnconsider->UnregisterFromLuaContext();

	m_behaviours.erase(std::remove_if(m_behaviours.begin(), m_behaviours.end(), [p_toUnconsider](ECS::Components::Behaviour* p_behaviour)
	{
		return p_toUnconsider == p_behaviour;
	}), m_behaviours.end());

	RefreshAll();
}

void AmberCore::Scripting::ScriptInterpreter::RefreshAll()
{
	DestroyLuaContext();
	CreateLuaContextAndBindGlobals();
}

bool AmberCore::Scripting::ScriptInterpreter::IsOk() const
{
	return m_isOk;
}
