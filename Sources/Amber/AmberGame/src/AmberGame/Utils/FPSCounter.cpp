#include "Amberpch.h"

#include "AmberGame/Utils/FPSCounter.h"

AmberGame::Utils::FPSCounter::FPSCounter(AmberWindowing::Context::Window& p_window) : m_window(p_window)
{
	m_defaultHorizontalAlignment = AmberUI::EHorizontalAlignment::RIGHT;
	m_defaultPosition = { static_cast<float>(m_window.GetSize().first) - 10.0f, 10.0f };

	m_text = &CreateWidget<AmberUI::Widgets::TextColored>("999 FPS", AmberRendering::Data::Color::Yellow);
}

void AmberGame::Utils::FPSCounter::Update(float p_deltaTime)
{
	m_elapsed += p_deltaTime;

	++m_frames;

	if (m_elapsed >= 0.1f)
	{
		m_text->Content = std::to_string(static_cast<int>(1.f / (m_elapsed / static_cast<float>(m_frames)))) + " FPS";
		SetPosition({ static_cast<float>(m_window.GetSize().first) - 10.0f, 10.0f });
		SetAlignment(AmberUI::EHorizontalAlignment::RIGHT, AmberUI::EVerticalAlignment::TOP);

		m_elapsed = 0.f;
		m_frames = 0;
	}
}
