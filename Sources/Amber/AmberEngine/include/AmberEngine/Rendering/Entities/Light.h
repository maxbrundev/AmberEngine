#pragma once

#include "AmberEngine/Maths/Transform.h"

#include "AmberEngine/Rendering/Settings/ELightType.h"

#include "AmberEngine/Data/Color.h"

namespace AmberEngine::Rendering::Entities
{
	struct Light
	{
		Light(Maths::Transform& p_transform, Settings::ELightType p_type);

		glm::mat4 GenerateMatrix(bool isStoringRotation = false) const;

		const Maths::Transform& GetTransform() const;
		float GetEffectRange() const;
		Data::Color Color = { 1.0f, 1.0f, 1.0f };
		float Intensity   = 1.0f;
		float Constant    = 0.0f;
		float Linear      = 0.0f;
		float Quadratic   = 1.0f;
		float Cutoff      = 12.0f;
		float OuterCutoff = 15.0f;
		float Type        = 0.0f;

	private:
		Maths::Transform& m_transform;
	};
}
