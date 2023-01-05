#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Utils
{
	class API_AMBERENGINE Maths
	{
	public:
		Maths() = delete;
		template<typename T>
		static std::tuple<T, T, T, T> Unpack(uint32_t p_value)
		{
			static_assert(std::is_fundamental_v<T>);

			return std::make_tuple(static_cast<T>((p_value >> 24) & 0xff),
				                   static_cast<T>((p_value >> 16) & 0xff),
				                   static_cast<T>((p_value >> 8)  & 0xff),
				                   static_cast<T>((p_value)       & 0xff));
		}

		static uint32_t Pack(uint8_t p_value1, uint8_t p_value2, uint8_t p_value3, uint8_t p_value4);
		static std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> Unpack(uint32_t p_value);
		
		static float ToDegree(float p_radians);
		static float ToRadians(float p_degree);
	};
}
