#include "Amberpch.h"

#include "AmberEngine/Rendering/Entities/Light.h"

AmberEngine::Rendering::Entities::Light::Light(Maths::Transform& p_transform, Settings::ELightType p_type) : Type(static_cast<float>(p_type)), m_transform(p_transform)
{
}

glm::mat4 AmberEngine::Rendering::Entities::Light::GenerateMatrix() const
{
	glm::mat4 result;

	const auto position = m_transform.GetWorldPosition();
	result[0][0] = position.x;
	result[0][1] = position.y;
	result[0][2] = position.z;

	const auto forward = m_transform.GetWorldForward();
	result[1][0] = forward.x;
	result[1][1] = forward.y;
	result[1][2] = forward.z;

	result[2][0] = static_cast<float>(Color.Pack());

	result[3][0] = Type;
	result[3][1] = Cutoff;
	result[3][2] = OuterCutoff;

	result[0][3] = Constant;
	result[1][3] = Linear;
	result[2][3] = Quadratic;
	result[3][3] = Intensity;

	return result;
}
