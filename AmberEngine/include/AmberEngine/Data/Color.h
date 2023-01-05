#pragma once

#include "AmberEngine/API/Export.h"

namespace AmberEngine::Data
{
	struct API_AMBERENGINE Color
	{
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color White;
		static const Color Black;
		static const Color Grey;
		static const Color Yellow;
		static const Color Cyan;
		static const Color Magenta;

		static const Color EditorYellow;

		float r;
		float g;
		float b;
		float a;

		Color(float p_r = 1.0f, float p_g = 1.0f, float p_b = 1.0f, float p_a = 1.0f);

		bool operator==(const Color& p_other) const;
		bool operator!=(const Color& p_other) const;
	};
}
