#include "Amberpch.h"

#include "AmberEngine/UI/Panels/APanel.h"

#include "AmberEngine/Data/Constants.h"
#include "AmberEngine/Tools/Utils/Converter.h"

AmberEngine::UI::APanel::APanel(const std::string& p_name, bool p_opened): isOpen(p_opened), name(p_name), size(256, 144)
{}

void AmberEngine::UI::APanel::CopyImGuiSize()
{
	size = glm::vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
}

std::pair<uint16_t, uint16_t> AmberEngine::UI::APanel::GetSafeSize() const
{
	const auto result = GetSize() - glm::vec2(0.0f, Data::Constants::EDITOR_FONT_SIZE_MEDIUM + ImGui::GetStyle().FramePadding.y  * 2);
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

void AmberEngine::UI::APanel::SetSize(const glm::vec2& p_size)
{
	size = p_size;
	isSizeChanged = true;
}

const glm::vec2 AmberEngine::UI::APanel::GetSize() const
{
	return size;
}

void AmberEngine::UI::APanel::DrawImplementation()
{
	ImGui::Begin("Device", nullptr);
	ImGui::TextColored(Utils::Converter::ToImVec4(Data::Color::EditorYellow), "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Begin(name.c_str(), nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);

	isFocused = ImGui::IsWindowFocused();
	isHovered = ImGui::IsWindowHovered();

	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

	if (!isFirstFrame)
	{
		CopyImGuiSize();
	}
	isFirstFrame = false;

	DrawContentImplementation();

	ImGui::End();
}
