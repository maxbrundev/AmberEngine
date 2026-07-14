#include "Amberpch.h"

#ifdef _DEBUG

#include "AmberGame/Debug/FrameInfo.h"

AmberGame::Debug::FrameInfo::FrameInfo(AmberCore::Rendering::ECSRenderer& p_renderer, AmberWindowing::Context::Window& p_window) :
	m_renderer(p_renderer),
	m_window(p_window)
{
	m_defaultHorizontalAlignment = AmberUI::EHorizontalAlignment::LEFT;
	m_defaultVerticalAlignment = AmberUI::EVerticalAlignment::BOTTOM;
	m_defaultPosition.x = 10.f;
	m_defaultPosition.y = static_cast<float>(p_window.GetSize().second) - 10.f;

	m_frameInfo[0] = &CreateWidget<AmberUI::Widgets::TextColored>("", AmberRendering::Data::Color::Yellow);
	m_frameInfo[1] = &CreateWidget<AmberUI::Widgets::TextColored>("", AmberRendering::Data::Color::Yellow);
	m_frameInfo[2] = &CreateWidget<AmberUI::Widgets::TextColored>("", AmberRendering::Data::Color::Yellow);
}

void AmberGame::Debug::FrameInfo::Update(float p_deltaTime)
{
	auto& frameInfo = m_renderer.GetFrameInfo();

	m_frameInfo[0]->Content = "Triangles: " + std::to_string(frameInfo.polyCount);
	m_frameInfo[1]->Content = "Batches: " + std::to_string(frameInfo.batchCount);
	m_frameInfo[2]->Content = "Instances: " + std::to_string(frameInfo.instanceCount);

	SetPosition({ 10.0f, static_cast<float>(m_window.GetSize().second) - 10.f });
	SetAlignment(AmberUI::EHorizontalAlignment::LEFT, AmberUI::EVerticalAlignment::BOTTOM);
}

#endif
