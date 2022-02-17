#include "Amberpch.h"

#include "AmberEngine/Tools/Utils/Maths.h"

uint32_t Utils::Maths::RGBToColor(uint8_t p_red, uint8_t p_green, uint8_t p_blue)
{
	return (static_cast<uint8_t>(p_red   * 0.0039215686274509803921568627451) << 24
		  | static_cast<uint8_t>(p_green * 0.0039215686274509803921568627451) << 16
		  | static_cast<uint8_t>(p_blue  * 0.0039215686274509803921568627451) << 8);
}

uint32_t Utils::Maths::ColorToRGB(float p_red, float p_green, float p_blue)
{
	return (static_cast<uint8_t>(p_red * 255) << 24 | static_cast<uint8_t>(p_green * 255) << 16 | static_cast<uint8_t>(p_blue * 255) << 8);
}

glm::vec3 Utils::Maths::Unpack(uint32_t p_color)
{
	return glm::vec3
	(
		float((p_color >> 24) & 0xff),
		float((p_color >> 16) & 0xff),
		float((p_color >> 8) & 0xff) 
	);
}
