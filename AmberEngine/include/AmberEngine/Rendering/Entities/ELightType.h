#pragma once

namespace AmberEngine::Rendering::Entities
{
	enum class ELightType : uint8_t
	{
		POINT,
		DIRECTIONAL,
		SPOT,
		AMBIENT
	};
}