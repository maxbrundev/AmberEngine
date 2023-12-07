#include "Amberpch.h"

#include "AmberEngine/Core/Editor.h"

#include "AmberEngine/Core/PanelsManager.h"
#include "AmberEngine/Core/ECS/Components/CModelRenderer.h"


#include "AmberEngine/UI/Panels/MenuBar.h"
#include "AmberEngine/UI/Panels/Hierarchy.h"
#include "AmberEngine/UI/Panels/Views/SceneView.h"
#include "AmberEngine/UI/Panels/Inspector.h"
#include "AmberEngine/UI/Panels/DriverInfoPanel.h"

#include "AmberEngine/Tools/Global/ServiceLocator.h"

AmberEngine::Core::Editor::Editor(Context& p_context) :
m_context(p_context), m_editorRenderer(p_context)
{
	Tools::Global::ServiceLocator::Provide(*this);

	m_context.renderer->RegisterModelMatrixSender([this](const glm::mat4& p_modelMatrix)
	{
		m_context.engineUBO->SetSubData(p_modelMatrix, 0);
	});

	m_panelsManager = std::make_unique<PanelsManager>(m_canvas);

	InitializeUI();
}

AmberEngine::Core::Editor::~Editor()
{
	m_context.sceneManager.UnloadCurrentScene();
}

void AmberEngine::Core::Editor::PreUpdate() const
{
	m_context.device->PollEvents();
	m_context.renderer->SetClearColor(0.1f, 0.1f, 0.1f);
	m_context.renderer->Clear(true, true, true);
}

void AmberEngine::Core::Editor::Update(float p_deltaTime)
{
	PrepareRendering(p_deltaTime);
	RenderViews(p_deltaTime);
	RenderEditorUI(p_deltaTime);
}

void AmberEngine::Core::Editor::PrepareRendering(float p_deltaTime)
{
	//m_context.engineUBO->SetSubData(m_context.device->GetElapsedTime(), 3 * sizeof(glm::mat4) + sizeof(glm::vec3));
}

void AmberEngine::Core::Editor::RenderEditorUI(float p_deltaTime)
{
	m_editorRenderer.RenderUI();
}

void AmberEngine::Core::Editor::PostUpdate() const
{
	PostRenderUI();
	m_context.window->SwapBuffers();
	m_context.inputManager->clearEvents();
}


void AmberEngine::Core::Editor::RenderViews(float p_deltaTime) const
{
	auto& sceneView = m_panelsManager->GetPanelAs<UI::SceneView>("Scene View");
	
	sceneView.Update(p_deltaTime);

	m_context.lightSSBO->Bind(0);

	if(sceneView.IsOpened())
		sceneView.Render();

	m_context.lightSSBO->Unbind();
}

void AmberEngine::Core::Editor::PostRenderUI() const
{
	m_context.uiManager->PostRender();
}

void AmberEngine::Core::Editor::InitializeUI()
{
	UI::Panels::PanelSettings settings;
	settings.closable    = true;
	settings.collapsable = true;
	settings.dockable    = true;

	m_panelsManager->CreatePanel<UI::Panels::MenuBar>("MenuBar");
	m_panelsManager->CreatePanel<UI::SceneView>("Scene View", true, settings);
	m_panelsManager->CreatePanel<UI::Panels::Hierarchy>("Hierarchy", true, settings);
	m_panelsManager->CreatePanel<UI::Panels::Inspector>("Inspector", true, settings);
	m_panelsManager->CreatePanel<UI::Panels::DriverInfoPanel>("Driver Info", true, settings);

	auto& hierarchy = m_panelsManager->GetPanelAs<UI::Panels::Hierarchy>("Hierarchy");
	auto& inspector = m_panelsManager->GetPanelAs<UI::Panels::Inspector>("Inspector");
	
	hierarchy.SelectActorEvent += [&](ECS::Actor& actor)
	{
		inspector.FocusActor(std::ref(actor));
	};

	m_canvas.MakeDockspace(true);
	m_context.uiManager->SetCanvas(m_canvas);
}

AmberEngine::Core::EditorRenderer& AmberEngine::Core::Editor::GetRenderer()
{
	return m_editorRenderer;
}
