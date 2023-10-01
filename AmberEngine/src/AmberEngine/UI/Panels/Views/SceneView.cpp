#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Views/SceneView.h"

#include "AmberEngine/Core/Editor.h"
#include "AmberEngine/Core/Renderer.h"

#include "AmberEngine/Tools/Utils/ServiceLocator.h"

AmberEngine::UI::SceneView::SceneView(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) :
	AView(p_title, p_opened, p_panelSettings),
	m_cameraController(m_camera, m_cameraPosition)
{
	m_cameraController.GetCamera().SetClearColor({ 0.098f, 0.098f, 0.098f });
	m_cameraController.GetCamera().SetFar(5000.0f);
}

void AmberEngine::UI::SceneView::Update(float p_deltaTime)
{
	AView::Update(p_deltaTime);

	if(IsHovered())
	{
		m_cameraController.Update(p_deltaTime);
	}
}

void AmberEngine::UI::SceneView::RenderImplementation()
{
	m_frameBuffer.Bind();
	Utils::ServiceLocator::Get<Core::Renderer>().Clear(m_cameraController.GetCamera(), true, true, true);
	Utils::ServiceLocator::Get<Core::Editor>().RenderScene();
}

void AmberEngine::UI::SceneView::DrawContent()
{
	auto[winWidth, winHeight] = GetSafeSize();

	const uint64_t textureID = m_frameBuffer.GetTextureID();

	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ static_cast<float>(winWidth), static_cast<float>(winHeight) }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
}

AmberEngine::LowRenderer::CameraController& AmberEngine::UI::SceneView::GetCameraController()
{
	return m_cameraController;
}