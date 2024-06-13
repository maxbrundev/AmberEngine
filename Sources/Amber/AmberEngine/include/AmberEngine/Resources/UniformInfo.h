#pragma once

#include <string>
#include <any>

#include "AmberEngine/Resources/EUniformType.h"

namespace AmberEngine::Resources
{
	struct UniformInfo
	{
		EUniformType type;
		std::string  name;
		uint32_t     location;
		std::any     defaultValue;
	};
}
