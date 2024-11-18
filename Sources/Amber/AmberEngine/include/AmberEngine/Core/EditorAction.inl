#pragma once

#include "AmberEngine/Core/EditorAction.h"

namespace AmberEngine::Core
{
	template<typename T>
	inline ECS::Actor & EditorAction::CreateMonoComponentActor(bool p_focusOnCreation, ECS::Actor* p_parent)
	{
		auto& instance = CreateEmptyActor(false, p_parent);

		T& component = instance.AddComponent<T>();

		instance.SetName(component.GetName());

		if (p_focusOnCreation)
			SelectActor(instance);

		return instance;
	}
}