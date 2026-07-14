#include "Amberpch.h"

#include "AmberEditor/Panels/AView.h"

#include "AmberWindowing/Context/Window.h"

#include "AmberEditor/Core/Context.h"
#include "AmberEditor/Core/Editor.h"

#include "AmberTools/Global/ServiceLocator.h"

AmberEditor::Panels::AView::AView(const std::string& p_title, bool p_opened, AmberUI::Panels::PanelSettings p_panelSettings) :
AmberUI::Panels::APanelWindow(p_title, p_opened, p_panelSettings),
m_frameBuffer(256, 144),
m_editorRenderer(AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::Editor>().GetRenderer())
{
	m_cameraPosition = { -10.0f, 3.0f, 10.0f };
	m_cameraRotation = glm::quat({ 0.0f, 135.0f, 0.0f });

	m_image = &CreateWidget<AmberUI::Widgets::Image>(m_frameBuffer.GetTextureID(), glm::vec2{ 0.f, 0.f });
	Settings.Scrollable = false;
}

AmberEditor::Panels::AView::~AView()
{
	m_frameBuffer.Unbind();
}

void AmberEditor::Panels::AView::PrepareCamera()
{
	auto[winWidth, winHeight] = GetSafeSize();
	m_camera.ComputeMatrices(winWidth, winHeight, m_cameraPosition, m_cameraRotation);
}

void AmberEditor::Panels::AView::FillEngineUBO()
{
	size_t offset = sizeof(glm::mat4); // We skip the model matrix (Which is a special case, modified every draw calls)
	AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::Context>().engineUBO->SetSubData(m_camera.GetViewMatrix(), offset);
	AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::Context>().engineUBO->SetSubData(m_camera.GetProjectionMatrix(), offset);
	AmberTools::Global::ServiceLocator::Get<AmberEditor::Core::Context>().engineUBO->SetSubData(m_cameraPosition, offset);
}

void AmberEditor::Panels::AView::ResizeFrameBuffer(uint16_t p_width, uint16_t p_height)
{
	m_frameBuffer.Resize(p_width, p_height);
}

void AmberEditor::Panels::AView::Update(float p_deltaTime)
{
	auto[winWidth, winHeight] = GetSafeSize();

	if (m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f
		&& (m_frameBuffer.GetSize().first != winWidth || m_frameBuffer.GetSize().second != winHeight))
	{
		m_image->Size = glm::vec2(static_cast<float>(winWidth), static_cast<float>(winHeight));
		ResizeFrameBuffer(winWidth, winHeight);
	}
}

void AmberEditor::Panels::AView::DrawImplementation()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(0, 0));

	AmberUI::Panels::APanelWindow::DrawImplementation();

	ImGui::PopStyleVar();
}

void AmberEditor::Panels::AView::Render()
{
	FillEngineUBO();

	auto[winWidth, winHeight] = GetSafeSize();
	AmberTools::Global::ServiceLocator::Get<AmberWindowing::Context::Window>().SetViewport(winWidth, winHeight);

	EDITOR_CONTEXT(shapeDrawer)->SetViewProjection(m_camera.GetProjectionMatrix() * m_camera.GetViewMatrix());

	RenderImplementation();
}

void AmberEditor::Panels::AView::SetCameraPosition(const glm::vec3& p_position)
{
	m_cameraPosition = p_position;
}

void AmberEditor::Panels::AView::SetCameraRotation(const glm::quat& p_rotation)
{
	m_cameraRotation = p_rotation;
}

const glm::vec3& AmberEditor::Panels::AView::GetCameraPosition() const
{
	return m_cameraPosition;
}

const glm::quat& AmberEditor::Panels::AView::GetCameraRotation() const
{
	return m_cameraRotation;
}

AmberRendering::Entities::Camera& AmberEditor::Panels::AView::GetCamera()
{
	return m_camera;
}
