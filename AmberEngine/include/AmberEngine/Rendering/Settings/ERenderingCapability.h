#pragma once

namespace AmberEngine::Rendering::Settings
{
	enum class ERenderingCapability
	{
		BLEND                    = 0x0BE2,
		CULL_FACE                = 0x0B44,
		DEPTH_TEST               = 0x0B71,
		DITHER                   = 0x0BD0,
		POLYGON_OFFSET_FILL      = 0x8037,
		SAMPLE_ALPHA_TO_COVERAGE = 0x809E,
		SAMPLE_COVERAGE          = 0x80A0,
		SCISSOR_TEST             = 0x0C11,
		STENCIL_TEST             = 0x0B90,
		MULTISAMPLE              = 0x809D
	};
}