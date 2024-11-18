#pragma once

#include "AmberEngine/Core/API/IInspectorItem.h"

namespace AmberEngine::Core::ECS
{
	class Actor;
}

namespace AmberEngine::Core::ECS::Components
{
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

		virtual std::string GetName() = 0;

		virtual void OnInspector(UI::WidgetContainer& p_root) override = 0;

	public:
		Actor& Owner;
	};
}
