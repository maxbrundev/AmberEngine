#include "Amberpch.h"

#include "AmberEngine/Tools/Utils/Converter.h"

ImVec4 AmberEngine::Utils::Converter::ToImVec4(const Data::Color& p_value)
{
	return ImVec4(p_value.r, p_value.g, p_value.b, p_value.a);
}

AmberEngine::Data::Color AmberEngine::Utils::Converter::ToColor(const ImVec4& p_value)
{
	return Data::Color(p_value.x, p_value.y, p_value.z, p_value.w);
}

ImVec2 AmberEngine::Utils::Converter::ToImVec2(const glm::vec2& p_value)
{
	return ImVec2(p_value.x, p_value.y);
}

glm::vec2 AmberEngine::Utils::Converter::ToFVector2(const ImVec2& p_value)
{
	return glm::vec2(p_value.x, p_value.y);
}
