#pragma once

#include "AmberMaths/Transform.h"

#include "AmberRendering/Settings/ELightType.h"

#include "AmberRendering/Data/Color.h"

namespace AmberRendering::Entities
{
	struct Light
	{
		Light(AmberMaths::Transform& p_transform, Settings::ELightType p_type);

		glm::mat4 GenerateMatrix(bool isStoringRotation = false) const;

		const AmberMaths::Transform& GetTransform() const;
		float GetEffectRange() const;
		AmberRendering::Data::Color Color = { 1.0f, 1.0f, 1.0f };
		float Intensity   = 1.0f;
		float Constant    = 0.0f;
		float Linear      = 0.0f;
		float Quadratic   = 1.0f;
		float Cutoff      = 12.0f;
		float OuterCutoff = 15.0f;
		float Type        = 0.0f;

	private:
		AmberMaths::Transform& m_transform;
	};
}
