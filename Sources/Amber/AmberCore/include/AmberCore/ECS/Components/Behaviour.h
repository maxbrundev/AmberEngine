#pragma once

#include <sol.hpp>

#include "AmberTools/Eventing/Event.h"

#include "AmberCore/ECS/Components/AComponent.h"

namespace AmberCore::ECS
{
	class Actor;
}

namespace AmberCore::ECS::Components
{
	class Behaviour : public AComponent
	{
	public:
		Behaviour(ECS::Actor& p_owner, const std::string& p_name);
		virtual ~Behaviour() override;

		virtual std::string GetName() override;

		bool RegisterToLuaContext(sol::state& p_luaState, const std::string& p_scriptFolder);
		void UnregisterFromLuaContext();

		template<typename... Args>
		void LuaCall(const std::string& p_functionName, Args&&... p_args);

		sol::table& GetTable();

		virtual void OnAwake() override;
		virtual void OnStart() override;
		virtual void OnEnable() override;
		virtual void OnDisable() override;
		virtual void OnDestroy() override;
		virtual void OnUpdate(float p_deltaTime) override;
		virtual void OnFixedUpdate(float p_deltaTime) override;
		virtual void OnLateUpdate(float p_deltaTime) override;
		virtual void OnCollisionEnter(Components::CPhysicalObject& p_otherObject) override;
		virtual void OnCollisionStay(Components::CPhysicalObject& p_otherObject) override;
		virtual void OnCollisionExit(Components::CPhysicalObject& p_otherObject) override;
		virtual void OnTriggerEnter(Components::CPhysicalObject& p_otherObject) override;
		virtual void OnTriggerStay(Components::CPhysicalObject& p_otherObject) override;
		virtual void OnTriggerExit(Components::CPhysicalObject& p_otherObject) override;

		virtual void OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node) override;
		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override;

	public:
		static AmberTools::Eventing::Event<Behaviour*> CreatedEvent;
		static AmberTools::Eventing::Event<Behaviour*> DestroyedEvent;

		const std::string Name;

	private:
		sol::table m_object = sol::nil;
	};
}

#include "AmberCore/ECS/Components/Behaviour.inl"
