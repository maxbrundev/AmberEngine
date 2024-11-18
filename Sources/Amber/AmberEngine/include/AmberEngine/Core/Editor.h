#pragma once

#include "AmberEngine/API/Export.h"

#include "AmberEngine/Core/Context.h"
#include "AmberEngine/Core/EditorRenderer.h"
#include "AmberEngine/Core/PanelsManager.h"
#include "AmberEngine/Core/EditorAction.h"

#include "AmberEngine/UI/Canvas.h"

namespace AmberEngine::Core
{
	class API_AMBERENGINE Editor final
	{
	public:
		Editor(Context& p_context);
		~Editor();

		Editor(const Editor& other)            = delete;
		Editor(Editor&& other)                 = delete;
		Editor& operator=(const Editor& other) = delete;
		Editor& operator=(Editor&& other)      = delete;

		void PreUpdate();
		void Update(float p_deltaTime);
		void PrepareRendering(float p_deltaTime);
		void RenderEditorUI(float p_deltaTime);
		void UpdateEditorPanels(float p_deltaTime);
		void PostUpdate();
		void UpdateCurrentEditorMode(float p_deltaTime);
		void RenderViews(float p_deltaTime);
		void PostRenderUI() const;
		void InitializeUI();
		void UpdatePlayMode(float p_deltaTime);
		void UpdateEditMode(float p_deltaTime);
		EditorRenderer& GetRenderer();

	public:
		Context& m_context;
		EditorRenderer m_editorRenderer;
		PanelsManager m_panelsManager;
		Core::EditorAction	m_editorActions;
		UI::Canvas m_canvas;

		uint64_t m_elapsedFrames = 0;
	};
}
