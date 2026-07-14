#include "Amberpch.h"

#include "AmberUI/Widgets/Image.h"

AmberUI::Widgets::Image::Image(uint32_t p_textureID, const glm::vec2& p_size) : TextureID{ p_textureID }, Size(p_size)
{
}

void AmberUI::Widgets::Image::DrawImplementation()
{
	ImGui::Image(TextureID.raw, ImVec2(Size.x, Size.y), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
}
