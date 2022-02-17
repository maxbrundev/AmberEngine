#include "Amberpch.h"

#include "AmberEngine/Rendering/Entities/Light.h"

uint32_t Pack(uint8_t c0, uint8_t c1, uint8_t c2, uint8_t c3)
{
	return (c0 << 24) | (c1 << 16) | (c2 << 8) | c3;
}

uint32_t Pack(const glm::vec3& p_toPack)
{
	return Pack(static_cast<uint8_t>(p_toPack.x * 255.f), static_cast<uint8_t>(p_toPack.y * 255.f), static_cast<uint8_t>(p_toPack.z * 255.f), 0);
}


AmberEngine::Rendering::Entities::Light::Light(Maths::Transform& p_tranform, ELightType p_type) : type(static_cast<float>(p_type)), m_transform(p_tranform)
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

	result[2][0] = static_cast<float>(Pack(color));

	result[3][0] = type;
	result[3][1] = cutoff;
	result[3][2] = outerCutoff;

	result[0][3] = constant;
	result[1][3] = linear;
	result[2][3] = quadratic;
	result[3][3] = intensity;

	return result;
}
