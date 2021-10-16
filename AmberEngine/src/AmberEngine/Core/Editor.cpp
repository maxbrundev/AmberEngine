#include "Amberpch.h"

#include "AmberEngine/Core/Editor.h"

AmberEngine::Core::Editor::Editor(Context& p_context) :
	m_context(p_context),
	m_sceneView(*p_context.m_window, *p_context.m_inputManager),
	isCameraFree(true)
{
	m_context.m_uiManager->EnableDocking(true);
}

void AmberEngine::Core::Editor::PreUpdate()
{
	m_sceneView.BindFBO();

	m_context.m_device->PollEvents();

	m_context.m_renderer->SetClearColor(0.1f, 0.1f, 0.1f);
	m_context.m_renderer->Clear(true, true, true);
}

void AmberEngine::Core::Editor::Update(float p_deltaTime)
{
	m_context.m_renderer->UpdateRenderMode();

	m_sceneView.PrepareCamera();

	m_sceneView.Update(p_deltaTime);
	UpdateInput();
}

void AmberEngine::Core::Editor::PostUpdate()
{
	m_context.m_window->SwapBuffers();
	m_context.m_inputManager->clearEvents();
}

void AmberEngine::Core::Editor::RenderScene()
{
	m_sceneView.UnbindFBO();
	m_context.m_uiManager->PreDraw();
	m_sceneView.Render();
	m_context.m_uiManager->PostDraw();
}

void AmberEngine::Core::Editor::UpdateInput()
{
	if (isCameraFree)
		FreeCamera();
	else
		LockCamera();

	if (m_context.m_inputManager->IsKeyPressed(Inputs::EKey::KEY_LEFT_SHIFT))
		m_context.m_renderer->ToggleWireFrame();

	if (m_context.m_inputManager->IsKeyPressed(Inputs::EKey::KEY_LEFT_ALT))
		ToggleCamera();

	if (m_context.m_inputManager->IsKeyPressed(Inputs::EKey::KEY_ESCAPE))
		m_context.m_window->SetShouldClose(true);
}

void AmberEngine::Core::Editor::FreeCamera()
{
	m_sceneView.GetCameraController().Unlock();
	m_context.m_window->SetCursorModeLock();
}

void AmberEngine::Core::Editor::LockCamera()
{
	m_sceneView.GetCameraController().Lock();
	m_context.m_window->SetCursorModeFree();
}

void AmberEngine::Core::Editor::ToggleCamera()
{
	isCameraFree = !isCameraFree;
}

void AmberEngine::Core::Editor::SetCameraPosition(const glm::vec3& p_position)
{
	m_sceneView.GetCameraController().SetPosition(p_position);
}
