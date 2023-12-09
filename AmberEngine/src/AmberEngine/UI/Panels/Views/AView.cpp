#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Views/AView.h"

#include "AmberEngine/Context/Window.h"

#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Core/Editor.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"

AmberEngine::UI::Panels::AView::AView(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) :
APanelWindow(p_title, p_opened, p_panelSettings),
m_cameraPosition(0.0f),
m_frameBuffer(256, 144),
m_editorRenderer(Tools::Global::ServiceLocator::Get<AmberEngine::Core::Editor>().GetRenderer())
{
	m_image = &CreateWidget<Widgets::Image>(m_frameBuffer.GetTextureID(), glm::vec2{ 0.f, 0.f });
}

AmberEngine::UI::Panels::AView::~AView()
{
	m_frameBuffer.Unbind();
}

void AmberEngine::UI::Panels::AView::PrepareCamera()
{
	auto[winWidth, winHeight] = GetSafeSize();
	m_camera.CalculateMatrices(winWidth, winHeight, m_cameraPosition);
}

void AmberEngine::UI::Panels::AView::FillEngineUBO()
{
	size_t offset = sizeof(glm::mat4); // We skip the model matrix (Which is a special case, modified every draw calls)
	AmberEngine::Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().engineUBO->SetSubData(m_camera.GetViewMatrix(), offset);
	Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().engineUBO->SetSubData(m_camera.GetProjectionMatrix(), offset);
	Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().engineUBO->SetSubData(m_cameraPosition, offset);
}

void AmberEngine::UI::Panels::AView::ResizeFrameBuffer(uint16_t p_width, uint16_t p_height)
{
	m_frameBuffer.Resize(p_width, p_height);
}

void AmberEngine::UI::Panels::AView::Update(float p_deltaTime)
{
	auto[winWidth, winHeight] = GetSafeSize();

	if (m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f
		&& (m_frameBuffer.GetSize().first != winWidth || m_frameBuffer.GetSize().second != winHeight))
	{
		m_image->size = glm::vec2(static_cast<float>(winWidth), static_cast<float>(winHeight));
		ResizeFrameBuffer(winWidth, winHeight);
	}
}

void AmberEngine::UI::Panels::AView::DrawImplementation()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(0, 0));

	APanelWindow::DrawImplementation();

	ImGui::PopStyleVar();
}

void AmberEngine::UI::Panels::AView::Render()
{
	FillEngineUBO();

	auto[winWidth, winHeight] = GetSafeSize();
	Tools::Global::ServiceLocator::Get<Context::Window>().SetViewport(winWidth, winHeight);

	RenderImplementation();
}