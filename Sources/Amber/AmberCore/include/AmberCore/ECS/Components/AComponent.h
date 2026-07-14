#pragma once

#include "AmberCore/API/IInspectorItem.h"

namespace AmberCore::ECS
{
	class Actor;
}

namespace AmberCore::ECS::Components
{
	class CPhysicalObject;

	class AComponent : public API::IInspectorItem
	{
	public:
		AComponent(Actor& p_owner);
		virtual ~AComponent() = default;

		virtual void OnAwake() {}

		virtual void OnStart() {}

		virtual void OnEnable() {}

		virtual void OnDisable() {}

		virtual void OnDestroy() {}

		virtual void OnUpdate(float p_deltaTime) {}

		virtual void OnFixedUpdate(float p_deltaTime) {}

		virtual void OnLateUpdate(float p_deltaTime) {}

		virtual void OnCollisionEnter(Components::CPhysicalObject& p_otherObject) {}

		virtual void OnCollisionStay(Components::CPhysicalObject& p_otherObject) {}

		virtual void OnCollisionExit(Components::CPhysicalObject& p_otherObject) {}

		virtual void OnTriggerEnter(Components::CPhysicalObject& p_otherObject) {}

		virtual void OnTriggerStay(Components::CPhysicalObject& p_otherObject) {}

		virtual void OnTriggerExit(Components::CPhysicalObject& p_otherObject) {}

		virtual std::string GetName() = 0;

		virtual void OnInspector(AmberUI::WidgetContainer& p_root) override = 0;

	public:
		Actor& Owner;
	};
}
