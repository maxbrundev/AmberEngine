#pragma once

namespace AmberEngine::Rendering::Settings
{
	enum class EComparisonAlgorithm
	{
		NEVER         = 0x0200,
		LESS          = 0x0201,
		EQUAL         = 0x0202,
		LESS_EQUAL    = 0x0203,
		GREATER       = 0x0204,
		NOTEQUAL      = 0x0205,
		GREATER_EQUAL = 0x0206,
		ALWAYS        = 0x0207
	};
}