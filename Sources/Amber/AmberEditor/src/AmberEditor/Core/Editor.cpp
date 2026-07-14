#include "Amberpch.h"

#include "AmberEditor/Core/Editor.h"

#include "AmberTools/Analytics/Profiling/Profiler.h"
#include "AmberTools/Analytics/Profiling/ProfilerSpy.h"
#include "AmberCore/SceneSystem/Scene.h"
#include "AmberEditor/Data/EditorConstants.h"
#include "AmberEditor/Panels/MenuBar.h"
#include "AmberEditor/Panels/Hierarchy.h"
#include "AmberEditor/Panels/SceneView.h"
#include "AmberEditor/Panels/Inspector.h"
#include "AmberEditor/Panels/FrameInfo.h"
#include "AmberEditor/Panels/DriverInfoPanel.h"

#include "AmberTools/Global/ServiceLocator.h"
#include "AmberEditor/Panels/AssetBrowser.h"
#include "AmberEditor/Panels/AssetProperties.h"
#include "AmberEditor/Panels/Console.h"
#include "AmberEditor/Panels/HardwareInfoPanel.h"
#include "AmberEditor/Panels/MaterialEditor.h"
#include "AmberEditor/Panels/ProfilerPanel.h"
#include "AmberEditor/Panels/ProjectSettings.h"
#include "AmberEditor/Panels/Toolbar.h"
#include "AmberEditor/Panels/AssetView.h"
#include "AmberEditor/Panels/GameView.h"
#include "AmberEditor/Panels/HelpWindow.h"

AmberEditor::Core::Editor::Editor(Context& p_context) :
m_context(p_context),
m_editorRenderer(p_context),
m_panelsManager(m_canvas),
m_editorActions(m_context, m_editorRenderer, m_panelsManager)
{
	AmberTools::Global::ServiceLocator::Provide(*this);

	InitializeUI();

	m_context.sceneManager.LoadEmptyLightedScene();
}

AmberEditor::Core::Editor::~Editor()
{
	m_context.sceneManager.UnloadCurrentScene();
}

void AmberEditor::Core::Editor::PreUpdate()
{
	PROFILER_SPY("Editor Pre-Update");

	m_context.device->PollEvents();
	m_context.renderer->SetClearColor(0.0f, 0.0f, 0.0f);
	m_context.renderer->Clear();
	m_context.renderer->ClearFrameInfo();
}

void AmberEditor::Core::Editor::Update(float p_deltaTime)
{
	UpdateCurrentEditorMode(p_deltaTime);
	PrepareRendering(p_deltaTime);
	RenderViews(p_deltaTime);
	UpdateEditorPanels(p_deltaTime);
	RenderEditorUI(p_deltaTime);

	m_editorActions.ExecuteDelayedActions();
}

void AmberEditor::Core::Editor::PrepareRendering(float p_deltaTime)
{
	PROFILER_SPY("Engine UBO Update");
	m_context.engineUBO->SetSubData(m_context.device->GetElapsedTime(), 3 * sizeof(glm::mat4) + sizeof(glm::vec3));
}

void AmberEditor::Core::Editor::RenderEditorUI(float p_deltaTime)
{
	PROFILER_SPY("Editor UI Rendering");

	m_editorRenderer.RenderUI();
}

void AmberEditor::Core::Editor::UpdateEditorPanels(float p_deltaTime)
{
	const auto& frameInfo = m_panelsManager.GetPanelAs<AmberEditor::Panels::FrameInfo>(Data::EditorConstants::EDITOR_PANEL_FRAME_INFO_TITLE);
	auto& hardwareInfo = m_panelsManager.GetPanelAs<AmberEditor::Panels::HardwareInfoPanel>("Hardware Info");
	auto& profiler = m_panelsManager.GetPanelAs<AmberEditor::Panels::ProfilerPanel>("Profiler");

	auto& sceneView = m_panelsManager.GetPanelAs<AmberEditor::Panels::SceneView>("Scene View");

	if (m_elapsedFrames == 1) // Let the first frame happen and then make the scene view the first seen view
		sceneView.Focus();

	if (frameInfo.IsOpened())
	{
		PROFILER_SPY("Frame Info Update");
		frameInfo.Update(p_deltaTime);
	}

	if (profiler.IsOpened())
	{
		PROFILER_SPY("Profiler Update");
		profiler.Update(p_deltaTime);
	}

	if (hardwareInfo.IsOpened())
	{
		PROFILER_SPY("Hardware Info Update");
		hardwareInfo.Update(p_deltaTime);
	}
}

void AmberEditor::Core::Editor::PostUpdate()
{
	PROFILER_SPY("Editor Post-Update");

	PostRenderUI();
	m_context.window->SwapBuffers();
	m_context.inputManager->clearEvents();

	++m_elapsedFrames;
}

void AmberEditor::Core::Editor::UpdateCurrentEditorMode(float p_deltaTime)
{
	if (auto editorMode = m_editorActions.GetCurrentEditorMode(); editorMode == EditorAction::EEditorMode::PLAY || editorMode == EditorAction::EEditorMode::FRAME_BY_FRAME)
		UpdatePlayMode(p_deltaTime);
	else
		UpdateEditMode(p_deltaTime);

	{
		PROFILER_SPY("Scene garbage collection");
		m_context.sceneManager.GetCurrentScene()->CollectGarbage();
		m_context.sceneManager.Update();
	}
}

void AmberEditor::Core::Editor::RenderViews(float p_deltaTime)
{
	auto& sceneView = m_panelsManager.GetPanelAs<AmberEditor::Panels::SceneView>(Data::EditorConstants::EDITOR_PANEL_SCENE_VIEW_TITLE);
	auto& gameView  = m_panelsManager.GetPanelAs<AmberEditor::Panels::GameView>("Game View");
	auto& assetView = m_panelsManager.GetPanelAs<AmberEditor::Panels::AssetView>("Asset View");

	{
		PROFILER_SPY("Editor Views Update");

		assetView.Update(p_deltaTime);
		gameView.Update(p_deltaTime);
		sceneView.Update(p_deltaTime);
	}

	if (assetView.IsOpened())
	{
		PROFILER_SPY("Asset View Rendering");

		m_context.simulatedLightSSBO->Bind(0);
		assetView.Render();
		m_context.simulatedLightSSBO->Unbind();
	}

	m_context.lightSSBO->Bind(0);
	
	if (gameView.IsOpened())
	{
		PROFILER_SPY("Game View Rendering");

		gameView.Render();
	}

	if (sceneView.IsOpened())
	{
		PROFILER_SPY("Scene View Rendering");

		sceneView.Render();
	}
	
	m_context.lightSSBO->Unbind();
}

void AmberEditor::Core::Editor::PostRenderUI() const
{
	m_context.uiManager->PostRender();
}

void AmberEditor::Core::Editor::InitializeUI()
{
	AmberUI::Panels::PanelSettings settings;
	settings.Closable    = true;
	settings.Dockable    = true;

	m_panelsManager.CreatePanel<AmberEditor::Panels::MenuBar>(Data::EditorConstants::EDITOR_PANEL_MENU_BAR_TITLE);
	m_panelsManager.CreatePanel<AmberEditor::Panels::AssetBrowser>("Asset Browser", true, settings, m_context.engineAssetsPath, m_context.projectAssetsPath, m_context.projectScriptsPath);
	m_panelsManager.CreatePanel<AmberEditor::Panels::SceneView>(Data::EditorConstants::EDITOR_PANEL_SCENE_VIEW_TITLE, true, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::GameView>("Game View", true, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::Hierarchy>(Data::EditorConstants::EDITOR_PANEL_HIERARCHY_TITLE, true, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::Inspector>(Data::EditorConstants::EDITOR_PANEL_INSPECTOR_TITLE, true, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::FrameInfo>(Data::EditorConstants::EDITOR_PANEL_FRAME_INFO_TITLE, true, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::HelpWindow>("Help", false, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::ProfilerPanel>("Profiler", true, settings, 0.25f);
	m_panelsManager.CreatePanel<AmberEditor::Panels::DriverInfoPanel>("Driver Info", true, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::MaterialEditor>("Material Editor", false, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::AssetView>("Asset View", false, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::HardwareInfoPanel>("Hardware Info", false, settings, 0.1f, 50);
	m_panelsManager.CreatePanel<AmberEditor::Panels::AssetProperties>("Asset Properties", false, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::Console>("Console", true, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::Toolbar>("Toolbar", true, settings);
	m_panelsManager.CreatePanel<AmberEditor::Panels::ProjectSettings>("Project Settings", false, settings);
	auto& hierarchy = m_panelsManager.GetPanelAs<AmberEditor::Panels::Hierarchy>(Data::EditorConstants::EDITOR_PANEL_HIERARCHY_TITLE);
	auto& inspector = m_panelsManager.GetPanelAs<AmberEditor::Panels::Inspector>(Data::EditorConstants::EDITOR_PANEL_INSPECTOR_TITLE);
	
	hierarchy.SelectActorEvent += [&](AmberCore::ECS::Actor& actor)
	{
		inspector.FocusActor(std::ref(actor));
	};
	
	m_canvas.MakeDockspace(true);
	m_context.uiManager->SetCanvas(m_canvas);
}

void AmberEditor::Core::Editor::UpdatePlayMode(float p_deltaTime)
{
	auto currentScene = m_context.sceneManager.GetCurrentScene();
	bool simulationApplied = false;

	{
		PROFILER_SPY("Physics Update");
		simulationApplied = m_context.physicsEngine->Update(p_deltaTime);
	}

	if (simulationApplied)
	{
		PROFILER_SPY("FixedUpdate");
		currentScene->FixedUpdate(p_deltaTime);
	}

	{
		PROFILER_SPY("Update");
		currentScene->Update(p_deltaTime);
	}

	{
		PROFILER_SPY("LateUpdate");
		currentScene->LateUpdate(p_deltaTime);
	}

	{
		PROFILER_SPY("Audio Update");
		m_context.audioEngine->Update();
	}


	ImGui::GetIO().DisableMouseUpdate = m_context.window->GetCursorMode() == AmberWindowing::Context::ECursorMode::DISABLED;

	if (m_editorActions.GetCurrentEditorMode() == EditorAction::EEditorMode::FRAME_BY_FRAME)
		m_editorActions.PauseGame();

	if (m_context.inputManager->IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_ESCAPE))
		m_editorActions.StopPlaying();
}

void AmberEditor::Core::Editor::UpdateEditMode(float p_deltaTime)
{
	if (m_context.inputManager->IsKeyPressed(AmberWindowing::Inputs::EKey::KEY_F5))
		m_editorActions.StartPlaying();
}

AmberEditor::Core::EditorRenderer& AmberEditor::Core::Editor::GetRenderer()
{
	return m_editorRenderer;
}
