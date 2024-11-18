#include "Amberpch.h"

#include "AmberEngine/UI/Panels/Views/GameView.h"

#include "AmberEngine/Core/EditorAction.h"

AmberEngine::UI::Panels::GameView::GameView(const std::string& p_title, bool p_opened, PanelSettings p_panelSettings) : AView(p_title, p_opened, p_panelSettings), m_sceneManager(EDITOR_CONTEXT(sceneManager))
{
}

void AmberEngine::UI::Panels::GameView::Update(float p_deltaTime)
{
	AView::Update(p_deltaTime);

	if (auto currentScene = EDITOR_CONTEXT(sceneManager).GetCurrentScene())
	{
		if (auto cameraComponent = EDITOR_CONTEXT(renderer)->FindMainCamera(*currentScene))
		{
			m_camera = cameraComponent->GetCamera();
			m_cameraPosition = cameraComponent->Owner.transform.GetWorldPosition();
			m_cameraRotation = cameraComponent->Owner.transform.GetWorldRotation();
			m_hasCamera = true;
			PrepareCamera();
		}
		else
		{
			m_camera.SetClearColor({ 0.f, 0.f, 0.f });
			m_hasCamera = false;
		}
	}
}

bool AmberEngine::UI::Panels::GameView::HasCamera() const
{
	return m_hasCamera;
}

void AmberEngine::UI::Panels::GameView::RenderImplementation()
{
	auto& baseRenderer = *EDITOR_CONTEXT(renderer).get();
	auto& currentScene = *m_sceneManager.GetCurrentScene();

	m_frameBuffer.Bind();

	baseRenderer.Clear(m_camera);

	uint8_t glState = baseRenderer.FetchGLState();
	baseRenderer.ApplyStateMask(glState);

	if (m_hasCamera)
	{
		m_editorRenderer.UpdateLights(currentScene);

		m_editorRenderer.RenderScene(m_cameraPosition, m_camera);
	}

	baseRenderer.ApplyStateMask(glState);

	m_frameBuffer.Unbind();
}
