#include "Amberpch.h"

#include "AmberEngine/Core/Editor.h"

AmberEngine::Core::Editor::Editor(Context& p_context) :
	m_context(p_context),
	m_sceneView(m_context),
	m_hierarchy("Hierarchy"),
	m_menuBar(m_context)
{
	m_context.renderer->RegisterModelMatrixSender([this](const glm::mat4& p_modelMatrix)
	{
		m_context.engineUBO->SetSubData(p_modelMatrix, 0);
	});

	m_context.uiManager->EnableDocking(true);
}

void AmberEngine::Core::Editor::PreUpdate() const
{
	m_context.device->PollEvents();

	m_context.renderer->SetClearColor(0.1f, 0.1f, 0.1f);
	m_context.renderer->Clear(true, true, true);

	m_context.uiManager->PreDraw();
}

void AmberEngine::Core::Editor::Update(float p_deltaTime)
{
	RenderViews(p_deltaTime);

	HandleInput();
}

void AmberEngine::Core::Editor::PostUpdate() const
{
	m_context.window->SwapBuffers();
	m_context.inputManager->clearEvents();
}

void AmberEngine::Core::Editor::RenderViews(float p_deltaTime)
{
	m_sceneView.Update(p_deltaTime);
	m_sceneView.Render();

	m_sceneView.Draw();
	m_hierarchy.Draw();
	m_menuBar.Draw();
	m_context.uiManager->PostDraw();
}

void AmberEngine::Core::Editor::HandleInput()
{
	if (m_context.inputManager->IsKeyPressed(Inputs::EKey::KEY_ESCAPE))
		m_context.window->SetShouldClose(true);
}
