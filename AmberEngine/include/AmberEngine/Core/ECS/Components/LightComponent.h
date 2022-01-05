#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/ECS/Components/AComponent.h"

#include "AmberEngine/Rendering/Entities/ELightType.h"
#include "AmberEngine/Rendering/Entities/Light.h"

namespace AmberEngine
{
	namespace ECS
	{
		class Actor;
	}
}

namespace AmberEngine::ECS::Components
{
	class API_AMBERENGINE LightComponent : public AComponent
	{
	public:
		LightComponent(Actor& p_owner, Rendering::Entities::ELightType p_type);
		virtual ~LightComponent() override;

		void Update(float p_deltaTime) override;

	private:
		Rendering::Entities::Light m_data;
	};
}