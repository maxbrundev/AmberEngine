#pragma once

#include "AmberEngine/Data/Color.h"

namespace AmberEngine::Utils
{
	class Converter
	{
	public:
		Converter() = delete;

		static ImVec4 ToImVec4(const Data::Color& p_value);

		static Data::Color ToColor(const ImVec4& p_value);

		static ImVec2 ToImVec2(const glm::vec2& p_value);

		static glm::vec2 ToFVector2(const ImVec2& p_value);
	};
}
