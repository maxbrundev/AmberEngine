#include "Amberpch.h"

#include "AmberEngine/Tools/Utils/Maths.h"

uint32_t AmberEngine::Utils::Maths::Pack(uint8_t p_value1, uint8_t p_value2, uint8_t p_value3, uint8_t p_value4)
{
	return (p_value1 << 24) | (p_value2 << 16) | (p_value3 << 8) | p_value4;
}

std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> AmberEngine::Utils::Maths::Unpack(uint32_t p_value)
{
	return std::make_tuple((p_value >> 24) & 0xff,
	                           (p_value >> 16) & 0xff,
		                       (p_value >> 8)  & 0xff,
		                       (p_value)       & 0xff);
}

float AmberEngine::Utils::Maths::ToDegree(float p_radians)
{
	return p_radians * ( 180.0f / glm::pi<float>());
}

float AmberEngine::Utils::Maths::ToRadians(float p_degree)
{
	return p_degree / (glm::pi<float>() * 180.0f);
}
