#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Rendering/Entities/ELightType.h"

#include "AmberEngine/Maths/Transform.h"

namespace AmberEngine::Rendering::Entities
{
	struct API_AMBERENGINE Light
	{
		Light(Maths::Transform& p_tranform, ELightType p_type);

		glm::vec3	color = { 1.0f, 1.0f, 1.0f };
		float		intensity = 1.f;
		float		constant = 0.0f;
		float		linear = 0.0f;
		float		quadratic = 1.0f;
		float		cutoff = 12.f;
		float		outerCutoff = 15.f;
		float		type = 0.0f;

	private:
		Maths::Transform& m_transform;
	};
}
