#include "Amberpch.h"

#include "AmberEngine/UI/Views/SceneView.h"

#include "AmberEngine/ImGUI/imgui_internal.h"

AmberEngine::UI::SceneView::SceneView(Context::Window& p_window, Inputs::InputManager& p_inputManager) :
	AView("Scene"),
	m_window(p_window),
	m_cameraController(p_window, p_inputManager, glm::vec3(0.0f, 0.0f, 15.0f)),
	m_frameBuffer(100, 100)//, viewportSize(1280, 720)
{
	m_frameBuffer.Resize(m_window.GetSize().first, m_window.GetSize().first);

	p_window.FramebufferResizeEvent.AddListener(std::bind(&SceneView::ResizeFrameBuffer, this, std::placeholders::_1, std::placeholders::_2));
}

AmberEngine::UI::SceneView::~SceneView()
{
	m_frameBuffer.Unbind();
}

void AmberEngine::UI::SceneView::Update(float p_deltaTime)
{
	m_cameraController.Update(p_deltaTime);
}

void AmberEngine::UI::SceneView::Render()
{
	ImGui::Begin("Device");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::SetNextWindowSizeConstraints(ImVec2(0,0), ImVec2(0, 0));

	ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
	//const float height = ImGui::GetCurrentWindow()->TitleBarHeight();
	//std::cout << height << std::endl;
	const uint64_t textureID = m_frameBuffer.GetTextureID();

	//ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	//viewportSize = { viewportPanelSize.x, viewportPanelSize.y };

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

	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ static_cast<float>(winWidth /*viewportSize.x*/), static_cast<float>(winHeight /*viewportSize.y*/) }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();
	ImGui::PopStyleVar();
}

void AmberEngine::UI::SceneView::PrepareCamera()
{
	auto[winWidth, winHeight] = GetSafeSize();
	m_cameraController.GetCamera().CalculateMatrices(winWidth /*viewportSize.x*/, winHeight /*viewportSize.y*/, m_cameraController.GetPosition());
}


void AmberEngine::UI::SceneView::BindFBO()
{
	auto[winWidth, winHeight] = GetSafeSize();
	m_frameBuffer.Resize(winWidth /*viewportSize.x*/, winHeight /*viewportSize.y*/);
	m_frameBuffer.Bind();
	m_window.SetViewport(winWidth /*viewportSize.x*/, winHeight /*viewportSize.y*/);
}

void AmberEngine::UI::SceneView::UnbindFBO()
{
	m_frameBuffer.Unbind();
}

AmberEngine::LowRenderer::CameraController& AmberEngine::UI::SceneView::GetCameraController()
{
	return m_cameraController;
}

void AmberEngine::UI::SceneView::ResizeFrameBuffer(uint16_t p_width, uint16_t p_height)
{
	m_frameBuffer.Resize(p_width, p_height);
}