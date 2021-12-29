#include "Amberpch.h"

#include "AmberEngine/Core/Editor.h"

AmberEngine::Core::Editor::Editor(Context& p_context) :
	m_context(p_context),
	m_sceneView(*p_context.window, *p_context.inputManager),
	isCameraFree(true)
{
	m_context.uiManager->EnableDocking(true);
}

void AmberEngine::Core::Editor::PreUpdate()
{
	m_sceneView.BindFBO();
	m_context.device->PollEvents();

	m_context.renderer->SetClearColor(0.1f, 0.1f, 0.1f);
	m_context.renderer->Clear(true, true, true);
	m_context.uiManager->PreDraw();

}

void AmberEngine::Core::Editor::Update(float p_deltaTime)
{
	m_context.renderer->UpdateRenderMode();

	m_sceneView.PrepareCamera();

	m_sceneView.Update(p_deltaTime);
	UpdateInput();
}

void AmberEngine::Core::Editor::PostUpdate()
{
	m_context.window->SwapBuffers();
	m_context.inputManager->clearEvents();
}

void AmberEngine::Core::Editor::RenderScene()
{
	m_sceneView.UnbindFBO();
	
	m_sceneView.Render();
	m_menuBar.Draw();
	m_context.uiManager->PostDraw();
}

void AmberEngine::Core::Editor::UpdateInput()
{
	if (isCameraFree)
		FreeCamera();
	else
		LockCamera();

	if (m_context.inputManager->IsKeyPressed(Inputs::EKey::KEY_LEFT_SHIFT))
		m_context.renderer->ToggleWireFrame();

	if (m_context.inputManager->IsKeyPressed(Inputs::EKey::KEY_LEFT_ALT))
		ToggleCamera();

	if (m_context.inputManager->IsKeyPressed(Inputs::EKey::KEY_ESCAPE))
		m_context.window->SetShouldClose(true);
}

void AmberEngine::Core::Editor::FreeCamera()
{
	m_sceneView.GetCameraController().Unlock();
	m_context.window->SetCursorModeLock();
}

void AmberEngine::Core::Editor::LockCamera()
{
	m_sceneView.GetCameraController().Lock();
	m_context.window->SetCursorModeFree();
}

void AmberEngine::Core::Editor::ToggleCamera()
{
	isCameraFree = !isCameraFree;
}

void AmberEngine::Core::Editor::SetCameraPosition(const glm::vec3& p_position)
{
	m_sceneView.GetCameraController().SetPosition(p_position);
}
