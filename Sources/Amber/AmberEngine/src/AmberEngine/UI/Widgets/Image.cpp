#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/Image.h"

AmberEngine::UI::Widgets::Image::Image(uint32_t p_textureID, const glm::vec2& p_size) : textureID{ p_textureID }, size(p_size)
{
}

void AmberEngine::UI::Widgets::Image::DrawImplementation()
{
	ImGui::Image(textureID.raw, ImVec2(size.x, size.y), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
}
