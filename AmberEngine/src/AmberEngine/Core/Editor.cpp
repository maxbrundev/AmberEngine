#include "Amberpch.h"

#include "AmberEngine/Core/Editor.h"

AmberEngine::Core::Editor::Editor(Context& p_context) : m_context(p_context), m_cameraController(*p_context.m_window, *p_context.m_inputManager, glm::vec3(0.0f, 0.0f, 3.0f)), isCameraFree(true)
{
}

AmberEngine::Core::Editor::~Editor()
{
}

void AmberEngine::Core::Editor::PreUpdate()
{
	m_context.m_device->PollEvents();
	m_context.m_renderer->SetClearColor(0.1f, 0.1f, 0.1f);
	m_context.m_renderer->Clear(true, true, false);
}

void AmberEngine::Core::Editor::Update(float p_deltaTime)
{
	m_context.m_renderer->UpdateRenderMode();
	m_cameraController.GetCamera().CalculateMatrices(m_context.m_window->GetSize().first, m_context.m_window->GetSize().second, m_cameraController.GetPosition());
	m_cameraController.Update(p_deltaTime);
	UpdateInput();
}

void AmberEngine::Core::Editor::PostUpdate()
{
	m_context.m_window->SwapBuffers();
	m_context.m_inputManager->clearEvents();
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
	m_cameraController.Unlock();
	m_context.m_window->SetCursorModeLock();
}

void AmberEngine::Core::Editor::LockCamera()
{
	m_cameraController.Lock();
	m_context.m_window->SetCursorModeFree();
}

void AmberEngine::Core::Editor::ToggleCamera()
{
	isCameraFree = !isCameraFree;
}

void AmberEngine::Core::Editor::SetCameraPosition(const glm::vec3& p_position)
{
	m_cameraController.SetPosition(p_position);
}

AmberEngine::LowRenderer::CameraController& AmberEngine::Core::Editor::GetCameraController()
{
	return m_cameraController;
}
