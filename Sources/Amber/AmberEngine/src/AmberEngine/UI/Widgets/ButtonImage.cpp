#include "Amberpch.h"

#include "AmberEngine/UI/Widgets/ButtonImage.h"

#include "AmberEngine/ImGUI/imgui_internal.h"
#include "AmberEngine/Tools/Utils/Converter.h"

AmberEngine::UI::Widgets::ButtonImage::ButtonImage(uint32_t p_textureID, const glm::vec2& p_size) : textureID{ p_textureID }, size(p_size)
{
}

void AmberEngine::UI::Widgets::ButtonImage::DrawImplementation()
{
	ImVec4 bg = Utils::Converter::ToImVec4(background);
	ImVec4 tn = Utils::Converter::ToImVec4(tint);

	if (ImGui::ImageButton(textureID.raw, Utils::Converter::ToImVec2(size), ImVec2(0.f, 1.f), ImVec2(1.f, 0.f), -1, bg, tn))
		ClickedEvent.Invoke();
}
