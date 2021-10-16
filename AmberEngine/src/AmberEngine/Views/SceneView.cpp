#include "Amberpch.h"

#include "AmberEngine/Views/SceneView.h"

AmberEngine::Views::SceneView::SceneView(Context::Window& p_window, Inputs::InputManager& p_inputManager) :
	AView("Scene"),
	m_window(p_window),
	m_cameraController(p_window, p_inputManager),
	m_frameBuffer(0, 0)
{
	m_frameBuffer.Resize(m_window.GetSize().first, m_window.GetSize().first);

	//p_window.FramebufferResizeEvent.AddListener(std::bind(&SceneView::ResizeFrameBuffer, this, std::placeholders::_1, std::placeholders::_2));
}

AmberEngine::Views::SceneView::~SceneView()
{
	m_frameBuffer.Unbind();
}

void AmberEngine::Views::SceneView::Update(float p_deltaTime)
{
	m_cameraController.Update(p_deltaTime);
}

void AmberEngine::Views::SceneView::Render()
{
	auto[winWidth, winHeight] = GetSafeSize();

	ImGui::Begin("Device");
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Begin(m_name.c_str());

	if (!isFirstFrame)
	{
		CopyImGuiSize();
	}
	isFirstFrame = false;

	const uint64_t textureID = m_frameBuffer.GetTextureID();

	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ static_cast<float>(winWidth), static_cast<float>(winHeight) }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

	ImGui::End();

	m_frameBuffer.Resize(winWidth, winHeight);

	m_window.SetViewport(winWidth, winHeight);
}

void AmberEngine::Views::SceneView::PrepareCamera()
{
	auto[winWidth, winHeight] = GetSafeSize();
	m_cameraController.GetCamera().CalculateMatrices(winWidth, winHeight, m_cameraController.GetPosition());
}


void AmberEngine::Views::SceneView::BindFBO()
{

	m_frameBuffer.Bind();
	
}

void AmberEngine::Views::SceneView::UnbindFBO()
{
	m_frameBuffer.Unbind();
}

void AmberEngine::Views::SceneView::ResizeFrameBuffer(uint16_t p_width, uint16_t p_height)
{
	m_frameBuffer.Resize(m_window.GetSize().first, m_window.GetSize().second);
}

AmberEngine::LowRenderer::CameraController& AmberEngine::Views::SceneView::GetCameraController()
{
	return m_cameraController;
}