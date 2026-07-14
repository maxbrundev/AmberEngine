#pragma once

#include <string>
#include <any>

#include "AmberRendering/Resources/EUniformType.h"

namespace AmberRendering::Resources
{
	struct UniformInfo
	{
		EUniformType type;
		std::string  name;
		uint32_t     location;
		std::any     defaultValue;
	};
}
