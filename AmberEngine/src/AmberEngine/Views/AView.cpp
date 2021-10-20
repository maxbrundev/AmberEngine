#include "Amberpch.h"

#include "AmberEngine/Views/AView.h"

AmberEngine::Views::AView::AView(std::string p_viewName) :
m_size(0, 0), m_name(std::move(p_viewName)), m_sizeChanged(false) {}

void AmberEngine::Views::AView::CopyImGuiSize()
{
	m_size = glm::vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
}

void AmberEngine::Views::AView::UpdateSize()
{
	if (m_sizeChanged)
	{
		ImGui::SetWindowSize(ImVec2(m_size.x, m_size.y), ImGuiCond_Always);
		m_sizeChanged = false;
	}
}

std::pair<uint16_t, uint16_t> AmberEngine::Views::AView::GetSafeSize() const
{
	const auto result = GetSize();
	return { static_cast<uint16_t>(result.x), static_cast<uint16_t>(result.y) };
}

const glm::vec2 AmberEngine::Views::AView::GetSize() const
{
	return m_size;
}

void AmberEngine::Views::AView::SetSize(const glm::vec2& p_size)
{
	m_size = p_size;
	m_sizeChanged = true;
}
