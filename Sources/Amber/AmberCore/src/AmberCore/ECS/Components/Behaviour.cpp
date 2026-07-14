#include "Amberpch.h"

#include "AmberCore/ECS/Components/Behaviour.h"

#include "AmberDebug/Logger.h"

#include "AmberUI/Widgets/TextColored.h"

#include "AmberCore/ECS/Actor.h"
#include "AmberCore/ECS/Components/CPhysicalObject.h"

AmberTools::Eventing::Event<AmberCore::ECS::Components::Behaviour*> AmberCore::ECS::Components::Behaviour::CreatedEvent;
AmberTools::Eventing::Event<AmberCore::ECS::Components::Behaviour*> AmberCore::ECS::Components::Behaviour::DestroyedEvent;

AmberCore::ECS::Components::Behaviour::Behaviour(ECS::Actor& p_owner, const std::string& p_name) :
	AComponent(p_owner),
	Name(p_name)
{
	CreatedEvent.Invoke(this);
}

AmberCore::ECS::Components::Behaviour::~Behaviour()
{
	DestroyedEvent.Invoke(this);
}

std::string AmberCore::ECS::Components::Behaviour::GetName()
{
	return "Behaviour";
}

bool AmberCore::ECS::Components::Behaviour::RegisterToLuaContext(sol::state& p_luaState, const std::string& p_scriptFolder)
{
	auto result = p_luaState.safe_script_file(p_scriptFolder + Name + ".lua", &sol::script_pass_on_error);

	if (!result.valid())
	{
		sol::error err = result;
		AB_LOG_ERROR(err.what());
		return false;
	}
	else
	{
		if (result.return_count() == 1 && result[0].is<sol::table>())
		{
			m_object = result[0];
			m_object["owner"] = &Owner;
			return true;
		}
		else
		{
			AB_LOG_ERROR("'" + Name + ".lua' missing return expression");
			return false;
		}
	}
}

void AmberCore::ECS::Components::Behaviour::UnregisterFromLuaContext()
{
	m_object = sol::nil;
}

sol::table& AmberCore::ECS::Components::Behaviour::GetTable()
{
	return m_object;
}

void AmberCore::ECS::Components::Behaviour::OnAwake()
{
	LuaCall("OnAwake");
}

void AmberCore::ECS::Components::Behaviour::OnStart()
{
	LuaCall("OnStart");
}

void AmberCore::ECS::Components::Behaviour::OnEnable()
{
	LuaCall("OnEnable");
}

void AmberCore::ECS::Components::Behaviour::OnDisable()
{
	LuaCall("OnDisable");
}

void AmberCore::ECS::Components::Behaviour::OnDestroy()
{
	LuaCall("OnDestroy");
}

void AmberCore::ECS::Components::Behaviour::OnUpdate(float p_deltaTime)
{
	LuaCall("OnUpdate", p_deltaTime);
}

void AmberCore::ECS::Components::Behaviour::OnFixedUpdate(float p_deltaTime)
{
	LuaCall("OnFixedUpdate", p_deltaTime);
}

void AmberCore::ECS::Components::Behaviour::OnLateUpdate(float p_deltaTime)
{
	LuaCall("OnLateUpdate", p_deltaTime);
}

void AmberCore::ECS::Components::Behaviour::OnCollisionEnter(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnCollisionStart", p_otherObject);
	LuaCall("OnCollisionEnter", p_otherObject);
}

void AmberCore::ECS::Components::Behaviour::OnCollisionStay(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnCollisionStay", p_otherObject);
}

void AmberCore::ECS::Components::Behaviour::OnCollisionExit(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnCollisionStop", p_otherObject);
	LuaCall("OnCollisionExit", p_otherObject);
}

void AmberCore::ECS::Components::Behaviour::OnTriggerEnter(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnTriggerStart", p_otherObject);
	LuaCall("OnTriggerEnter", p_otherObject);
}

void AmberCore::ECS::Components::Behaviour::OnTriggerStay(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnTriggerStay", p_otherObject);
}

void AmberCore::ECS::Components::Behaviour::OnTriggerExit(Components::CPhysicalObject& p_otherObject)
{
	LuaCall("OnTriggerStop", p_otherObject);
	LuaCall("OnTriggerExit", p_otherObject);
}

void AmberCore::ECS::Components::Behaviour::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void AmberCore::ECS::Components::Behaviour::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
{
}

void AmberCore::ECS::Components::Behaviour::OnInspector(AmberUI::WidgetContainer& p_root)
{
	if (m_object.valid())
	{
		p_root.CreateWidget<AmberUI::Widgets::TextColored>("Ready", AmberRendering::Data::Color::Green);
		p_root.CreateWidget<AmberUI::Widgets::TextColored>("Your script gets interpreted by the engine with success", AmberRendering::Data::Color::White);
	}
	else
	{
		p_root.CreateWidget<AmberUI::Widgets::TextColored>("Compilation failed!", AmberRendering::Data::Color::Red);
		p_root.CreateWidget<AmberUI::Widgets::TextColored>("Check the console for more information", AmberRendering::Data::Color::White);
	}
}
