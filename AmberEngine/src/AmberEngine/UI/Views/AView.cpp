#include "Amberpch.h"

#include "AmberEngine/UI/Views/AView.h"

AmberEngine::UI::AView::AView(std::string p_viewName) :
m_size(100, 100),
m_name(std::move(p_viewName)),
m_sizeChanged(false) {}

void AmberEngine::UI::AView::CopyImGuiSize()
{
	m_size = glm::vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
}

void AmberEngine::UI::AView::UpdateSize()
{
	if (m_sizeChanged)
	{
		ImGui::SetWindowSize(ImVec2(m_size.x, m_size.y), ImGuiCond_Always);
		m_sizeChanged = false;
	}
}

std::pair<uint16_t, uint16_t> AmberEngine::UI::AView::GetSafeSize() const
{
	const auto result = GetSize() - glm::vec2(0.0f, 19.0f);
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

const glm::vec2 AmberEngine::UI::AView::GetSize() const
{
	return m_size;
}

void AmberEngine::UI::AView::SetSize(const glm::vec2& p_size)
{
	m_size = p_size;
	m_sizeChanged = true;
}
