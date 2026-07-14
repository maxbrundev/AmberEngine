#include "Amberpch.h"

#include "AmberUI/Tools/Converter.h"

ImVec4 AmberUI::Tools::Converter::ToImVec4(const AmberRendering::Data::Color& p_value)
{
	return ImVec4(p_value.r, p_value.g, p_value.b, p_value.a);
}

AmberRendering::Data::Color AmberUI::Tools::Converter::ToColor(const ImVec4& p_value)
{
	return AmberRendering::Data::Color(p_value.x, p_value.y, p_value.z, p_value.w);
}

ImVec2 AmberUI::Tools::Converter::ToImVec2(const glm::vec2& p_value)
{
	return ImVec2(p_value.x, p_value.y);
}

glm::vec2 AmberUI::Tools::Converter::ToFVector2(const ImVec2& p_value)
{
	return glm::vec2(p_value.x, p_value.y);
}
