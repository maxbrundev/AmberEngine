#pragma once

#include "AmberEngine/API/Export.h"


#include "AmberEngine/Core/ECS/Components/CLight.h"

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
	class API_AMBERENGINE CDirectionalLight : public CLight
	{
	public:
		CDirectionalLight(Actor& p_owner);
		virtual ~CDirectionalLight() override = default;

		std::string GetName() override;

		virtual void OnInspector(UI::WidgetContainer& p_root) override;
	};
}
