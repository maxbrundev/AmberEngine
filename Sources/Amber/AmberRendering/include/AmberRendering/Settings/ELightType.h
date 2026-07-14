#pragma once

namespace AmberRendering::Settings
{
	enum class ELightType : uint8_t
	{
		POINT,
		DIRECTIONAL,
		AMBIENT_BOX,
		AMBIENT_SPHERE,
		SPOT,
	};
}