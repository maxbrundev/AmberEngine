#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Views/AView.h"

#include "AmberEngine/Context/Window.h"

#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Core/Editor.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"

AmberEngine::UI::Panels::AView::AView(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) :
APanelWindow(p_title, p_opened, p_panelSettings),
m_frameBuffer(256, 144),
m_editorRenderer(Tools::Global::ServiceLocator::Get<AmberEngine::Core::Editor>().GetRenderer())
{
	m_cameraPosition = { -10.0f, 3.0f, 10.0f };
	m_cameraRotation = glm::quat({ 0.0f, 135.0f, 0.0f });

	m_image = &CreateWidget<Widgets::Image>(m_frameBuffer.GetTextureID(), glm::vec2{ 0.f, 0.f });
	settings.scrollable = false;
}

AmberEngine::UI::Panels::AView::~AView()
{
	m_frameBuffer.Unbind();
}

void AmberEngine::UI::Panels::AView::PrepareCamera()
{
	auto[winWidth, winHeight] = GetSafeSize();
	m_camera.ComputeMatrices(winWidth, winHeight, m_cameraPosition, m_cameraRotation);
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

	EDITOR_CONTEXT(shapeDrawer)->SetViewProjection(m_camera.GetProjectionMatrix() * m_camera.GetViewMatrix());

	RenderImplementation();
}

void AmberEngine::UI::Panels::AView::SetCameraPosition(const glm::vec3& p_position)
{
	m_cameraPosition = p_position;
}

void AmberEngine::UI::Panels::AView::SetCameraRotation(const glm::quat& p_rotation)
{
	m_cameraRotation = p_rotation;
}

const glm::vec3& AmberEngine::UI::Panels::AView::GetCameraPosition() const
{
	return m_cameraPosition;
}

const glm::quat& AmberEngine::UI::Panels::AView::GetCameraRotation() const
{
	return m_cameraRotation;
}

AmberEngine::Rendering::Entities::Camera& AmberEngine::UI::Panels::AView::GetCamera()
{
	return m_camera;
}
