#pragma once

namespace AmberEngine::Rendering::Settings
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