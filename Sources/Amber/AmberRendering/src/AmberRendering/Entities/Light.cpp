#include "Amberpch.h"

#include "AmberRendering/Entities/Light.h"

AmberRendering::Entities::Light::Light(AmberMaths::Transform& p_transform, Settings::ELightType p_type) : Type(static_cast<float>(p_type)), m_transform(p_transform)
{
}

glm::mat4 AmberRendering::Entities::Light::GenerateMatrix(bool isStoringRotation) const
{
	glm::mat4 result;

	const auto position = m_transform.GetWorldPosition();
	result[0][0] = position.x;
	result[0][1] = position.y;
	result[0][2] = position.z;

	if(isStoringRotation)
	{
		const glm::quat& rotation = m_transform.GetWorldRotation();
		result[1][0] = rotation.x;
		result[1][1] = rotation.y;
		result[1][2] = rotation.z;
		result[2][1] = rotation.w;
	}
	else
	{
		const auto forward = m_transform.GetWorldForward();
		result[1][0] = forward.x;
		result[1][1] = forward.y;
		result[1][2] = forward.z;
	}

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

const AmberMaths::Transform& AmberRendering::Entities::Light::GetTransform() const
{
	return m_transform;
}

float CalculateLuminosity(float p_constant, float p_linear, float p_quadratic, float p_intensity, float p_distance)
{
	const float attenuation = (p_constant + p_linear * p_distance + p_quadratic * (p_distance * p_distance));
	return (1.0f / attenuation) * std::abs(p_intensity);
}

// Incremental Search
float CalculatePointLightRadius(float p_constant, float p_linear, float p_quadratic, float p_intensity)
{
	constexpr float minimumIllumination = 1.0f / 255.0f;

	float distance = 0.0f;

	if (CalculateLuminosity(p_constant, p_linear, p_quadratic, p_intensity, 1000.0f) > minimumIllumination)
	{
		return std::numeric_limits<float>::infinity();
	}

	while (CalculateLuminosity(p_constant, p_linear, p_quadratic, p_intensity, distance + 100.0f) > minimumIllumination)
	{
		distance += 100.0f;
	}

	while (CalculateLuminosity(p_constant, p_linear, p_quadratic, p_intensity, distance) > minimumIllumination)
	{
		distance+= 0.1f;
	}

	return distance;
}

// Quadratic Equation
float CalculatePointLightRadiusQuadratic(float p_constant, float p_linear, float p_quadratic, float p_intensity)
{
	constexpr float minimumIllumination = 1.0f / 255.0f;

	// https://learnopengl.com/Advanced-Lighting/Deferred-Shading -> Calculating a light's volume or radius.
	// std::abs(p_intensity) since we support negative intensity.
	const float radius = (-p_linear + std::sqrtf(p_linear * p_linear - 4 * p_quadratic * (p_constant - std::abs(p_intensity) / minimumIllumination))) / (2 * p_quadratic);

	return radius;
}

float CalculateAmbientBoxLightRadius(const glm::vec3& p_position, const  glm::vec3& p_size)
{
	return glm::distance(p_position, p_position + p_size);
}

float AmberRendering::Entities::Light::GetEffectRange() const
{
	switch (static_cast<Settings::ELightType>(static_cast<int>(Type)))
	{
	case Settings::ELightType::POINT:
	case Settings::ELightType::SPOT:			return CalculatePointLightRadiusQuadratic(Constant, Linear, Quadratic, Intensity);
	case Settings::ELightType::AMBIENT_BOX:		return CalculateAmbientBoxLightRadius(m_transform.GetWorldPosition(), { Constant, Linear, Quadratic });
	case Settings::ELightType::AMBIENT_SPHERE:	return Constant;
	}

	return std::numeric_limits<float>::infinity();
}
