#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Resources
{
	enum class API_AMBERENGINE EUniformType : uint16_t
	{
		UNIFORM_BOOL         = 0x8B56,
		UNIFORM_INT          = 0x1404,
		UNIFORM_FLOAT        = 0x1406,
		UNIFORM_FLOAT_VEC2   = 0x8B50,
		UNIFORM_FLOAT_VEC3   = 0x8B51,
		UNIFORM_FLOAT_VEC4   = 0x8B52,
		UNIFORM_FLOAT_MAT4   = 0x8B5C,
		UNIFORM_DOUBLE_MAT4  = 0x8F48,
		UNIFORM_SAMPLER_2D   = 0x8B5E,
		UNIFORM_SAMPLER_CUBE = 0x8B60
	};
}