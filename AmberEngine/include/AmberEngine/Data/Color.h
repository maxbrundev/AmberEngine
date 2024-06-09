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
		Color(uint32_t p_packedData);
		Color(const glm::vec3& p_standardColor);
		Color(const glm::vec4& p_standardColor);

		uint32_t Pack() const;

		std::tuple<float, float, float, float> GetStandardized();

		glm::vec3 GetVec3() const;
		glm::vec4 GetVec4() const;

		Color& operator+=(const Color& p_color);
		bool operator==(const Color& p_other) const;
		bool operator!=(const Color& p_other) const;

		static Color Mix(const Color& p_color1, const Color& p_color2, float p_alpha);
	};
} 