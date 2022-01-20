#include "Amberpch.h"

#include "AmberEngine/UI/Views/SceneView.h"

AmberEngine::UI::SceneView::SceneView(Core::Context& p_context) :
	AView("Scene"),
	m_context(p_context),
	m_cameraController(*m_context.window, *m_context.inputManager, glm::vec3(0.0f, 0.0f, 15.0f)),
	m_frameBuffer(256, 144)
	
{
	//m_context.window->FramebufferResizeEvent.AddListener([this](auto&& PH1, auto&& PH2)
	//{
	//	ResizeFrameBuffer(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2));
	//});
}

AmberEngine::UI::SceneView::~SceneView()
{
	m_frameBuffer.Unbind();
}

void AmberEngine::UI::SceneView::Update(float p_deltaTime)
{
	m_cameraController.Update(p_deltaTime);

	auto[winWidth, winHeight] = GetSafeSize();

	if (viewportSize.x > 0.0f && viewportSize.y > 0.0f 
		&& (m_frameBuffer.GetSize().first != winWidth || m_frameBuffer.GetSize().second != winHeight))
	{
		m_frameBuffer.Resize(winWidth, winHeight);
	}

	m_context.window->SetViewport(winWidth, winHeight);

	m_context.m_scene.Update(p_deltaTime);
}

void AmberEngine::UI::SceneView::Render()
{
	PrepareCamera();

	FillEngineUBO();

	m_frameBuffer.Bind();
	m_context.renderer->Clear(m_cameraController.GetCamera(), true, true, true);

	m_context.m_scene.DrawAll(*m_context.renderer);
}

void AmberEngine::UI::SceneView::Draw()
{
	ImGui::Begin("Device");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(0, 0));

	ImGui::Begin(m_name.c_str(), nullptr);

	//const float height = ImGui::GetCurrentWindow()->TitleBarHeight();
	// TODO: Clarify why is there a difference between viewportSize and GetSafeSize values since both exclude Title bar height.
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	//if(viewportPanelSize.x > 0 && viewportPanelSize.y > 0)
	//{
	//	viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
	//}

	//{
	//	ImVec2 vMin = ImGui::GetWindowContentRegionMin();
	//	ImVec2 vMax = ImGui::GetWindowContentRegionMax();
	//
	//	vMin.x += ImGui::GetWindowPos().x;
	//	vMin.y += ImGui::GetWindowPos().y;
	//	vMax.x += ImGui::GetWindowPos().x;
	//	vMax.y += ImGui::GetWindowPos().y;
	//
	//	ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255));
	//}

	if (!isFirstFrame)
	{
		CopyImGuiSize();
	}
	isFirstFrame = false;

	auto[winWidth, winHeight] = GetSafeSize();

	const uint64_t textureID = m_frameBuffer.GetTextureID();
	
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ static_cast<float>(winWidth), static_cast<float>(winHeight) }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
	ImGui::PopStyleVar();

	m_frameBuffer.Unbind();
}

void AmberEngine::UI::SceneView::PrepareCamera()
{
	auto[winWidth, winHeight] = GetSafeSize();
	m_cameraController.GetCamera().CalculateMatrices(winWidth, winHeight, m_cameraController.GetPosition());
}

void AmberEngine::UI::SceneView::FillEngineUBO()
{
	size_t offset = sizeof(glm::mat4); // We skip the model matrix (Which is a special case, modified every draw calls)
	m_context.engineUBO->SetSubData(m_cameraController.GetCamera().GetViewMatrix(), std::ref(offset));
	m_context.engineUBO->SetSubData(m_cameraController.GetCamera().GetProjectionMatrix(), std::ref(offset));
	m_context.engineUBO->SetSubData(m_cameraController.GetPosition(), std::ref(offset));
}

AmberEngine::LowRenderer::CameraController& AmberEngine::UI::SceneView::GetCameraController()
{
	return m_cameraController;
}

void AmberEngine::UI::SceneView::ResizeFrameBuffer(uint16_t p_width, uint16_t p_height)
{
	m_frameBuffer.Resize(p_width, p_height);
}