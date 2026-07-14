#pragma once

#include "AmberEditor/Core/EditorAction.h"

namespace AmberEditor::Core
{
	template<typename T>
	inline AmberCore::ECS::Actor & EditorAction::CreateMonoComponentActor(bool p_focusOnCreation, AmberCore::ECS::Actor* p_parent)
	{
		auto& instance = CreateEmptyActor(false, p_parent);

		T& component = instance.AddComponent<T>();

		instance.SetName(component.GetName());

		if (p_focusOnCreation)
			SelectActor(instance);

		return instance;
	}
}