#pragma once

#include "AmberEngine/API/Export.h"

namespace Utils
{
	class API_AMBERENGINE Maths
	{
	public:
		Maths() = delete;

		static uint32_t RGBToColor(uint8_t p_red, uint8_t p_green, uint8_t p_blue);
		static uint32_t ColorToRGB(float p_red, float p_green, float p_blue);
		static glm::vec3 Unpack(uint32_t p_color);
	};
}
