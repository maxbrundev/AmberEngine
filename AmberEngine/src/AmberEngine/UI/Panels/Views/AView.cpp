#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Views/AView.h"

#include "AmberEngine/Context/Window.h"

#include "AmberEngine/Core/Context.h"

#include "AmberEngine/Tools/Utils/ServiceLocator.h"

AmberEngine::UI::AView::AView(const std::string& p_title, bool p_opened) :
APanel(p_title, p_opened),
m_cameraPosition(0.0f),
m_frameBuffer(256, 144)
{}

AmberEngine::UI::AView::~AView()
{
	m_frameBuffer.Unbind();
}

void AmberEngine::UI::AView::UpdateSize()
{
	if (isSizeChanged)
	{
		ImGui::SetWindowSize(ImVec2(size.x, size.y), ImGuiCond_Always);
		isSizeChanged = false;
	}
}

void AmberEngine::UI::AView::PrepareCamera()
{
	auto[winWidth, winHeight] = GetSafeSize();
	m_camera.CalculateMatrices(winWidth, winHeight, m_cameraPosition);
}

void AmberEngine::UI::AView::FillEngineUBO()
{
	size_t offset = sizeof(glm::mat4); // We skip the model matrix (Which is a special case, modified every draw calls)
	Utils::ServiceLocator::Get<Core::Context>().engineUBO->SetSubData(m_camera.GetViewMatrix(), offset);
	Utils::ServiceLocator::Get<Core::Context>().engineUBO->SetSubData(m_camera.GetProjectionMatrix(), offset);
	Utils::ServiceLocator::Get<Core::Context>().engineUBO->SetSubData(m_cameraPosition, offset);
}

void AmberEngine::UI::AView::ResizeFrameBuffer(uint16_t p_width, uint16_t p_height)
{
	m_frameBuffer.Resize(p_width, p_height);
}

void AmberEngine::UI::AView::Update(float p_deltaTime)
{
	auto[winWidth, winHeight] = GetSafeSize();

	if (viewportSize.x > 0.0f && viewportSize.y > 0.0f
		&& (m_frameBuffer.GetSize().first != winWidth || m_frameBuffer.GetSize().second != winHeight))
	{
		ResizeFrameBuffer(winWidth, winHeight);
	}

	Utils::ServiceLocator::Get<Context::Window>().SetViewport(winWidth, winHeight);
}

void AmberEngine::UI::AView::DrawImplementation()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(0, 0));

	APanel::DrawImplementation();

	ImGui::PopStyleVar();

	m_frameBuffer.Unbind();
}

void AmberEngine::UI::AView::Render()
{
	PrepareCamera();

	FillEngineUBO();

	RenderImplementation();
}

void AmberEngine::UI::AView::Draw()
{
	DrawImplementation();
}





