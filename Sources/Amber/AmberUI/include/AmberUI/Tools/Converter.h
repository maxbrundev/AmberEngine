#pragma once

#include "AmberRendering/Data/Color.h"

namespace AmberUI::Tools
{
	class Converter
	{
	public:
		Converter() = delete;

		static ImVec4 ToImVec4(const AmberRendering::Data::Color& p_value);

		static AmberRendering::Data::Color ToColor(const ImVec4& p_value);

		static ImVec2 ToImVec2(const glm::vec2& p_value);

		static glm::vec2 ToFVector2(const ImVec2& p_value);
	};
}
