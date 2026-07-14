#include "Amberpch.h"

#include "AmberUI/Widgets/ButtonImage.h"

#include "imgui_internal.h"
#include "AmberUI/Tools/Converter.h"

AmberUI::Widgets::ButtonImage::ButtonImage(uint32_t p_textureID, const glm::vec2& p_size) : TextureID{ p_textureID }, Size(p_size)
{
}

void AmberUI::Widgets::ButtonImage::DrawImplementation()
{
	ImVec4 bg = AmberUI::Tools::Converter::ToImVec4(Background);
	ImVec4 tn = AmberUI::Tools::Converter::ToImVec4(Tint);

	if (ImGui::ImageButton(TextureID.raw, AmberUI::Tools::Converter::ToImVec2(Size), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), -1, bg, tn))
		ClickedEvent.Invoke();
}
