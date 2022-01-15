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
		virtual ~LightComponent() override = default;

		void Update(float p_deltaTime) override;

		void SetColor(const glm::vec3& p_color);
		void SetIntensity(float p_intensity);
		void SetConstant(float p_constant);
		void SetLinear(float p_linear);
		void SetQuadratic(float p_quadratic);
		void SetCutoff(float p_cutoff);
		void SetOuterCutoff(float p_outerCutoff);

		Rendering::Entities::Light& GetLightData();

	private:
		Rendering::Entities::Light m_data;
	};
}