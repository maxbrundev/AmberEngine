#pragma once

#include "AmberEngine/Core/API/IInspectorItem.h"

namespace AmberEngine
{
	namespace Core
	{
		namespace ECS
		{
			class Actor;
		}
	}
}
namespace AmberEngine::Core::ECS::Components
{
	class AComponent : public API::IInspectorItem
	{
	public:
		AComponent(Actor& p_owner);
		virtual ~AComponent() = default;

		virtual std::string GetName() = 0;
		virtual void OnInspector(UI::WidgetContainer& p_root) override = 0;

		Actor& owner;
	};
}
