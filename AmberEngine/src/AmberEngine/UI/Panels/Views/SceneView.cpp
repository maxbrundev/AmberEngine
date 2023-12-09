#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Views/SceneView.h"

#include "AmberEngine/Core/Editor.h"
#include "AmberEngine/Core/Renderer.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"

AmberEngine::UI::Panels::SceneView::SceneView(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) :
	AView(p_title, p_opened, p_panelSettings),
	m_sceneManager(Tools::Global::ServiceLocator::Get<AmberEngine::Core::SceneSystem::SceneManager>()),
	m_cameraController(m_camera, m_cameraPosition)
{
	m_cameraController.GetCamera().SetClearColor({ 0.098f, 0.098f, 0.098f });
	m_cameraController.GetCamera().SetFar(5000.0f);
}

void AmberEngine::UI::Panels::SceneView::Update(float p_deltaTime)
{
	AView::Update(p_deltaTime);

	if(IsHovered())
	{
		m_cameraController.Update(p_deltaTime);
	}
}

void AmberEngine::UI::Panels::SceneView::RenderScene()
{
	auto& baseRenderer = Tools::Global::ServiceLocator::Get<AmberEngine::Core::Context>().renderer;

	auto& currentScene = *m_sceneManager.GetCurrentScene();
	m_editorRenderer.UpdateLights(currentScene);

	m_frameBuffer.Bind();
	baseRenderer->Clear(m_camera);
	m_editorRenderer.RenderScene(m_cameraPosition, m_camera);

	m_frameBuffer.Unbind();
}

void AmberEngine::UI::Panels::SceneView::RenderImplementation()
{
	PrepareCamera();

	RenderScene();
}

AmberEngine::LowRenderer::CameraController& AmberEngine::UI::Panels::SceneView::GetCameraController()
{
	return m_cameraController;
}