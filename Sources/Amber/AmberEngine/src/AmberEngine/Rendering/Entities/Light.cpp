#include "Amberpch.h"

#include "AmberEngine/Rendering/Entities/Light.h"

AmberEngine::Rendering::Entities::Light::Light(Maths::Transform& p_transform, Settings::ELightType p_type) : Type(static_cast<float>(p_type)), m_transform(p_transform)
{
}

glm::mat4 AmberEngine::Rendering::Entities::Light::GenerateMatrix(bool isStoringRotation) const
{
	glm::mat4 result;

	const auto position = m_transform.GetWorldPosition();
	result[0][0] = position.x;
	result[0][1] = position.y;
	result[0][2] = position.z;

	if(isStoringRotation)
	{
		const auto rotationEuler = m_transform.GetWorldRotationEuler();
		result[1][0] = rotationEuler.x;
		result[1][1] = rotationEuler.y;
		result[1][2] = rotationEuler.z;
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

const AmberEngine::Maths::Transform& AmberEngine::Rendering::Entities::Light::GetTransform() const
{
	return m_transform;
}

float CalculateLuminosity(float p_constant, float p_linear, float p_quadratic, float p_intensity, float p_distance)
{
	auto attenuation = (p_constant + p_linear * p_distance + p_quadratic * (p_distance * p_distance));
	return (1.0f / attenuation) * std::abs(p_intensity);
}

float CalculatePointLightRadius(float p_constant, float p_linear, float p_quadratic, float p_intensity)
{
	constexpr float threshold = 1 / 255.0f;
	constexpr float step = 1.0f;

	float distance = 0.0f;

#define TRY_GREATER(value)\
	else if (CalculateLuminosity(p_constant, p_linear, p_quadratic, p_intensity, value) > threshold)\
	{\
		distance = value;\
	}

#define TRY_LESS(value, newValue)\
	else if (CalculateLuminosity(p_constant, p_linear, p_quadratic, p_intensity, value) < threshold)\
	{\
		distance = newValue;\
	}

	// Prevents infinite while true. If a light has a bigger radius than 10000 we ignore it and make it infinite
	if (CalculateLuminosity(p_constant, p_linear, p_quadratic, p_intensity, 1000.0f) > threshold)
	{
		return std::numeric_limits<float>::infinity();
	}
	TRY_LESS(20.0f, 0.0f)
		TRY_GREATER(750.0f)
		TRY_LESS(50.0f, 20.0f + step)
		TRY_LESS(100.0f, 50.0f + step)
		TRY_GREATER(500.0f)
		TRY_GREATER(250.0f)

		while (true)
		{
			if (CalculateLuminosity(p_constant, p_linear, p_quadratic, p_intensity, distance) < threshold) // If the light has a very low luminosity for the given distance, we consider the current distance as the light radius
			{
				return distance;
			}
			else
			{
				distance += step;
			}
		}
}

float CalculateAmbientBoxLightRadius(const glm::vec3& p_position, const  glm::vec3& p_size)
{
	return glm::distance(p_position, p_position + p_size);
}


float AmberEngine::Rendering::Entities::Light::GetEffectRange() const
{
	switch (static_cast<Settings::ELightType>(static_cast<int>(Type)))
	{
	case Settings::ELightType::POINT:
	case Settings::ELightType::SPOT:			return CalculatePointLightRadius(Constant, Linear, Quadratic, Intensity);
	case Settings::ELightType::AMBIENT_BOX:		return CalculateAmbientBoxLightRadius(m_transform.GetWorldPosition(), { Constant, Linear, Quadratic });
	case Settings::ELightType::AMBIENT_SPHERE:	return Constant;
	}

	return std::numeric_limits<float>::infinity();
}
